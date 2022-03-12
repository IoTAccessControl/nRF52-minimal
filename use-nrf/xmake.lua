includes("proj.lua")

set_project("nrf-demo")

set_plat("cross")
set_arch("cortex-m4")

toolchain("arm-gcc")
	local tool_path = "/Applications/ARM/"
	set_kind("cross")
	set_description("Stm32 Arm Embedded Compiler")
	set_sdkdir(tool_path)
	set_toolset("cc", "arm-none-eabi-gcc")
	set_toolset("ld", "arm-none-eabi-gcc")
	set_toolset("ar", "arm-none-eabi-ar")
	set_toolset("as", "arm-none-eabi-gcc")

	-- compile with the least flags
	local defs = " -DDEBUG -DNRF52840_XXAA -DNRFX_CLOCK_ENABLED=1 -DNRFX_UARTE_ENABLED=1 -DNRFX_UARTE0_ENABLED=1 "
	local cflags = defs.." -fno-common -ffreestanding -Os "
	cflags = cflags.." -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard "
	cflags = cflags.." -fdata-sections -ffunction-sections "

	cflags = cflags

	local lflags = "-lgcc "
	lflags = lflags.." -Tsrc/nrf52840.ld "
	lflags = lflags.."-specs=nano.specs	-lc -lm -lnosys -Wl,--gc-sections"

	-- TODO: gcc link和ld link的区别
	on_load(function (toolchain)
		toolchain:add("cxflags", cflags) 
		toolchain:add("ldflags", cflags) 
		toolchain:add("ldflags", lflags)
		toolchain:add("asflags", cflags)
	end)
toolchain_end()

set_toolchains("arm-gcc")

rule("gen-hex")
	after_build(function (target)
		print("after_build")
		local out = target:targetfile() or ""
		local bin_out = " build/"..target:name()..".hex"
		print(string.format("%s => %s", out, bin_out))
		-- os.exec("arm-none-eabi-objcopy -Obinary "..out.." "..bin_out)
		os.exec("arm-none-eabi-objcopy -O ihex "..out.." "..bin_out)	
		-- os.exec("nrfjprog -f nrf52 --program "..bin_out.." --sectorerase")
		-- os.exec("nrfjprog -f nrf52 --reset")
		-- os.exec("qemu-system-arm -M stm32-p103 -nographic -kernel"..bin_out)
	end)
rule_end()

local cmisis = "/Users/fripside/Tools/IoT-SDK/Nrf52840/nRF5_SDK_17.1.0_ddde560/components/toolchain/cmsis/include"
local nrfx = "/Users/fripside/Dev/Research/Compartment/nrf52840/nrf52480-baremetal/nrfx"

-- local defines = {"DEBUG", "NRF52840_XXAA", "NRFX_CLOCK_ENABLED=1", "NRFX_UARTE_ENABLED=1", "NRFX_UARTE0_ENABLED=1"}
-- add_defines(defines)
add_includedirs(nrfx)
add_includedirs(nrfx.."/mdk")
add_includedirs(nrfx.."/drivers/include")
add_includedirs("src")
-- add_includedirs()
-- add_includedirs(nrfx.."/templates/nRF52840")
add_includedirs(nrfx.."/templates")
add_includedirs(cmisis)
-- add_includedirs(nrfx + "")
target("nrfx-app")
	set_kind("binary")
	add_files(nrfx.."/drivers/src/*.c")
	-- add_files("src/startup_nrf52840.s")
	-- add_files({"src/*.c"})
	add_rules("gen-hex")
	add_files(files)
	after_build(function (target)
        print("after_build")
	    local out_fi = target:targetfile()
        local bin_out = path.join(target:targetdir(), target:name())
		if out_fi then
			print(string.format("%s => %s", out_fi, bin_out))
			-- os.exec("arm-none-eabi-objcopy -Obinary "..out.." "..bin_out)
			os.exec("arm-none-eabi-objcopy -O ihex "..out_fi.." "..bin_out..".hex")
			
		end
    end)
target_end()

task("flash")
    -- set the category for showing it in plugin category menu (optional)
    set_category("plugin")
    -- the main entry of the plugin
    on_run(function (target)
		os.exec("nrfjprog -f nrf52 --program build/nrfx-app.hex --sectorerase")
		os.exec("nrfjprog -f nrf52 --reset")
    end)

    -- set the menu options, but we put empty options now.
    set_menu {
                -- usage
                usage = "xmake hello [options]"

                -- description
            ,   description = "Flash the hex file to nrf52840!"
                -- options
            ,   options = {}
            }
task_end()
