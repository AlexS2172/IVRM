
EgTransportAdapterps.dll: dlldata.obj EgTransportAdapter_p.obj EgTransportAdapter_i.obj
	link /dll /out:EgTransportAdapterps.dll /def:EgTransportAdapterps.def /entry:DllMain dlldata.obj EgTransportAdapter_p.obj EgTransportAdapter_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del EgTransportAdapterps.dll
	@del EgTransportAdapterps.lib
	@del EgTransportAdapterps.exp
	@del dlldata.obj
	@del EgTransportAdapter_p.obj
	@del EgTransportAdapter_i.obj
