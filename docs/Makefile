MODULE = xdc
all:
	gtkdoc-scan --module=$(MODULE) --source-dir=..
	gtkdoc-mkdb --module=$(MODULE) --main-sgml-file=$(MODULE)-main-sgml-file
	gtkdoc-mkhtml wsevent $(MODULE)-main-sgml-file || exit 0

clean:
	rm -rf $(MODULE).* $(MODULE)-* *.html *sgml*
