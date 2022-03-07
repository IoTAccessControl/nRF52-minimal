
-- arm-gcc installing path
local tool_path = "/Applications/ARM/"

local cflags = {
	"-mcpu=cortex-m4",
	" -mthumb",
	"-mfloat-abi=hard  -mfpu=fpv4-sp-d16",
	"-fdata-sections -ffunction-sections",
	"-nostartfiles",
	"-Os",
}

local ldflags = {
	"-specs=nano.specs",
	"-lc",
	"-lm",
	"-lnosys",
	"-Wl,--gc-sections",
}

function use_toolchain(sdk_path)
	toolchain("arm-gcc")
		set_kind("cross")
		set_description("Stm32 Arm Embedded Compiler")
		set_sdkdir(sdk_path)
		set_toolset("cc", "arm-none-eabi-gcc")
		set_toolset("ld", "arm-none-eabi-gcc")
		set_toolset("ar", "arm-none-eabi-ar")
		set_toolset("as", "arm-none-eabi-gcc")
	toolchain_end()
	set_toolchains("arm-gcc")
end

rule("arm-gcc")
	on_load(function (target)
		-- target:add("defines", "-DTEST")
		-- force add ld flags, ldflags {force = true}
		target:set("policy", "check.auto_ignore_flags", false)
		target:add("cxflags", cflags)
		target:add("asflags", cflags)
		-- use gcc to link
		target:add("ldflags", cflags)
		target:add("ldflags", ldflags)
	end)
	
	after_build(function (target)
		print("after_build")
		local out = target:targetfile() or ""
		local bin_out = " build/minimal-proj.bin"
		print(string.format("%s => %s", out, bin_out))
		os.exec("arm-none-eabi-objcopy -Obinary "..out.." "..bin_out)
		-- os.exec("qemu-system-arm -M stm32-p103 -nographic -kernel"..bin_out)
	end)
rule_end()

task("flash")
	
task_end()