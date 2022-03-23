
local cflags = {
	"-O3 -g3",
	"-mcpu=cortex-m4",
	"-mthumb", 
	-- "-nostdlib", -- do not use NewLib
	--  "-mabi=aapcs",
	"-fstack-usage",
	"-ffreestanding",
	-- "-Wall -Werror",
	"-mfloat-abi=hard -mfpu=fpv4-sp-d16",
	-- keep every function in a separate section, this allows linker to discard unused ones
	"-ffunction-sections -fdata-sections -fno-strict-aliasing",
	"-fno-builtin -fshort-enums"
}

local ldflags = {
	"-specs=nano.specs",
	-- "-lc",
	"-lm",
	"-lnosys",
	"-Wl,--print-memory-usage",
	"-Wl,--gc-sections",
}

toolchain("arm-gcc")
	set_kind("cross")
	set_description("Stm32 Arm Embedded Compiler")
	-- set_sdkdir(sdk_path)
	set_toolset("cc", "arm-none-eabi-gcc")
	set_toolset("ld", "arm-none-eabi-gcc")
	set_toolset("ar", "arm-none-eabi-ar")
	set_toolset("as", "arm-none-eabi-gcc")

	-- on_load(function (target) 
		
	-- end)
toolchain_end()

rule("arm-gcc")
	on_load(function (target)
		-- target:add("defines", "-DTEST")
		-- force add ld flags, ldflags {force = true}
		-- target:set("policy", "check.auto_map_flags", false)
		target:set("policy", "check.auto_ignore_flags", true)
		target:add("cxflags", cflags)
		target:add("asflags", cflags)
		-- use gcc to link
		target:add("ldflags", cflags, {force = true})
		target:add("ldflags", ldflags, {force = true})
	end)

	before_build(function (target)
		-- print(target:get("includedirs"))
	end)
	
	after_build(function (target)
		if target:kind() == "binary" then
			print("Generate firmware")
			local out_fi = target:targetfile()
			local gen_fi = "build/"..target:name()
			local binutil = get_config("sdk").."/bin/arm-none-eabi-"
			if out_fi then
				print(string.format(binutil.." %s => %s", out_fi, gen_fi))
				os.cp(out_fi, gen_fi..".elf")
				-- os.exec(binutil.."objcopy -O ihex "..out_fi.." "..gen_fi..".hex")
				os.execv(binutil.."objdump", {"-S", out_fi}, {stdout=gen_fi..".asm"})
				-- os.exec(binutil.."objcopy -Obinary "..out_fi.." "..gen_fi..".bin")
				-- os.exec(binutil.."objcopy -I binary -O ihex "..gen_fi..".bin "..gen_fi.."2.hex")
			end
		end
	end)

	after_clean(function (target)
		local gen_fi = path.join("build", target:name())
		print("want to clean: "..gen_fi)
		os.rm(gen_fi.."*")
	end)
rule_end()

-- directly linked now
rule("link-app")
	add_deps("arm-gcc")	
	on_load(function (target)
		-- local nrf_sdx_path = get_config("NRF_SDK")
		-- local ld_dir = nrf_sdx_path.."/modules/nrfx/mdk"
		-- print("------------------------")
		-- print(ld_dir)
		-- target:add("ldflags", "-L"..ld_dir.." -Tshell_app/uart_gcc_nrf52.ld", {force=true})
		-- target:add("ldflags",  "-L"..ld_dir.." -Tshell_app/my_nrf52.ld", {force=true})
	end)
	
	after_build(function (target)
		print("Generate firmware")
		local out_fi = target:targetfile()
		local gen_fi = "build/"..target:name()
		local binutil = get_config("sdk").."/bin/arm-none-eabi-"
		if out_fi then
			print(string.format(binutil.." %s => %s", out_fi, gen_fi))
			os.cp(out_fi, gen_fi..".elf")
			os.exec(binutil.."objcopy -O ihex "..out_fi.." "..gen_fi..".hex")
			os.execv(binutil.."objdump", {"-S", out_fi}, {stdout=gen_fi..".asm"})
			-- os.exec(binutil.."objcopy -Obinary "..out_fi.." "..gen_fi..".bin")
			-- os.exec(binutil.."objcopy -I binary -O ihex "..gen_fi..".bin "..gen_fi.."2.hex")
		end
	end)
rule_end()


rule("link-all")
	add_deps("arm-gcc")
	after_build(function (target)
		print("================================================")
		print("Merge firmware")
		local binutil = get_config("sdk").."/bin/arm-none-eabi-"

		-- gen bin 1
		local bin_main = "build/nrf-app.elf"
		local gen_bin1 = "build/nrf-app.bin"
		os.exec(binutil.."objcopy --pad-to=0x04000 --gap-fill=0xFF -Obinary "..bin_main.." "..gen_bin1)
		print("generate: "..gen_bin1)
		-- --pad-to=0x4000 --gap-fill=0xFF -O binary 
		-- gen bin 2
		local bin_nrf = "build/nrf-sdk-bin.elf"
		local gen_bin2 = "build/nrf-sdk-bin.bin"
		os.exec(binutil.."objcopy -Obinary "..bin_nrf.." "..gen_bin2)
		print("generate: "..gen_bin2)

		-- merge two bin
		local merge_bin = "build/merge.bin"
		-- os.execv("cmd /c cat", gen_bin2, {stdout=gen_bin1})
		local file = io.open(merge_bin, "wb")
		local bin1 = io.open(gen_bin1, "rb")
		local bin2 = io.open(gen_bin2, "rb")
		print("add "..gen_bin1)
		-- file:write(io.readfile(gen_bin1, "rb"))
		file:write(bin1:read("*a"))
		file:write(bin2:read("*a"))
		-- print("add "..gen_bin2)
		-- file:write(io.readfile(gen_bin2, "rb"))
		bin1:close()
		bin2:close()
		file:close()

		-- gen hex
		local merge_hex = "build/merge.hex"
		os.exec(binutil.."objcopy -I binary -O ihex "..merge_bin.." "..merge_hex)
	end)
rule_end()

rule("link-test-lib")
	add_deps("arm-gcc")
	after_build(function (target)
		print("================================================")
		print("Merge firmware")
		local binutil = get_config("sdk").."/bin/arm-none-eabi-"

		-- gen bin 1
		local bin_main = "build/nrf-app.elf"
		local gen_bin1 = "build/nrf-app.bin"
		os.exec(binutil.."objcopy --pad-to=0x04000 --gap-fill=0xFF -Obinary "..bin_main.." "..gen_bin1)
		print("generate: "..gen_bin1)
		-- --pad-to=0x4000 --gap-fill=0xFF -O binary 
		-- gen bin 2
		local bin_nrf = "build/nrf-test-lib.elf"
		local gen_bin2 = "build/nrf-test-lib.bin"
		os.exec(binutil.."objcopy -Obinary "..bin_nrf.." "..gen_bin2)
		print("generate: "..gen_bin2)

		-- merge two bin
		local merge_bin = "build/merge.bin"
		-- os.execv("cmd /c cat", gen_bin2, {stdout=gen_bin1})
		local file = io.open(merge_bin, "wb")
		-- local bin1 = io.open(gen_bin1, "rb")
		-- local bin2 = io.open(gen_bin2, "rb")
		print("add "..gen_bin1)
		file:write(io.readfile(gen_bin1, {encoding = "binary"}))
		-- file:write(bin1:read("*a"))
		-- file:write(bin2:read("*a"))
		print("add "..gen_bin2)
		file:write(io.readfile(gen_bin2, {encoding = "binary"}))
		-- bin1:close()
		-- bin2:close()
		file:close()

		-- gen hex
		local merge_hex = "build/merge.hex"
		os.exec(binutil.."objcopy -I binary -O ihex "..merge_bin.." "..merge_hex)
	end)
rule_end()