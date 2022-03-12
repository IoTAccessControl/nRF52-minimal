option("NRF_SDK")
	set_default(nil)
	set_showmenu(true)
option_end()

local nrf_sdx_path = get_config("NRF_SDK") or ""

if nrf_sdx_path == "" then
	print("NRF_SDK is not set")
else
	
end
add_includedirs("")
