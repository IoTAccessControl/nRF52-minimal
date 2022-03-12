includes("armgcc.lua")
includes("nrf_files.lua")

set_toolchains("arm-gcc")

option("NRF_SDK")
	set_default("")
	set_showmenu(true)
option_end()

local nrf_lib_src = {}
local nrf_lib_inc = {}

do 
	local nrf_sdx_path = get_config("NRF_SDK") or ""
	-- print("Run build----------"..(nrf_sdx_path or "not set"))
	if nrf_sdx_path ~= "" then
		for _, p in ipairs(nrf_include_files) do
			-- add_includedirs(nrf_sdx_path..p)
			-- print(nrf_sdx_path..p)
			table.insert(nrf_lib_inc, nrf_sdx_path..p)
		end
		for _, p in ipairs(nrf_src_files) do
			table.insert(nrf_lib_src, nrf_sdx_path..p)
		end
	end
end

add_includedirs(nrf_lib_inc)
add_includedirs("../config")
add_defines(nrf_defines)

target("nrf-sdk-lib")
	set_kind("static")
	add_rules("arm-gcc")
	add_files(nrf_lib_src)
	add_files("usart_lib/*.c")
	-- add_files("src/uart_gcc_nrf52.ld")

-- gcc -l links with a library file.
-- gcc -L looks in directory for library files.
local app_name = "nrf-app"
target(app_name)
	set_kind("binary")
	add_rules("arm-gcc")
	add_deps("nrf-sdk-lib")
	add_files("shell_app/*.c")
	-- add_files("shell_app/uart_gcc_nrf52.ld")
	add_includedirs("shell_app")
	add_includedirs("channels")
	add_includedirs("usart_lib")
	add_files("channels/*.c")
	add_rules("link-app")

task("flash")
	-- set the category for showing it in plugin category menu (optional)
	set_category("plugin")
	-- the main entry of the plugin
	on_run(function ()
		local hex_fi = "build/"..app_name..".hex"
		print("install hex file: "..hex_fi)
		os.exec("nrfjprog -f nrf52 --program "..hex_fi.." --sectorerase")
		os.exec("nrfjprog -f nrf52 --reset")
	end)

	-- set the menu options, but we put empty options now.
	set_menu {
				usage = "xmake hello [options]",
				description = "Flash the hex file to nrf52840!",
				options = {}
			}
task_end()