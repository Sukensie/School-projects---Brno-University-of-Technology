# balíčky pro windows
# GNU make Windows http://gnuwin32.sourceforge.net/packages/make.htm
# GNU zip Windows http://gnuwin32.sourceforge.net/packages/zip.htm
# Doxygen Windows http://www.stack.nl/~dimitri/doxygen/download.html
# PHP windows https://windows.php.net/download#php-7.3

TEAM_NAME = xjanda28_xcolog00_xkocma08_xsouce15

.PHONY: run clean pack help profile doc
run: profile
	
clean:
	if exist doc rmdir doc

pack: doc
#	mkdir -p ../../$(TEAM_NAME)/doc && cp -a doc/. ../../$(TEAM_NAME)/doc
#	mkdir -p ../../$(TEAM_NAME)/install
#	mkdir -p ../../$(TEAM_NAME)/repo && cp -a ../ ../../$(TEAM_NAME)/repo
#	rm -rf ../../$(TEAM_NAME)/repo/src/doc
#	cd ../../ && zip $(TEAM_NAME).zip $(TEAM_NAME)
	mkdir -p ../../$(TEAM_NAME)/doc && xcopy /s doc/. ../../$(TEAM_NAME)/doc
	mkdir -p ../../$(TEAM_NAME)/install
	mkdir -p ../../$(TEAM_NAME)/repo && xcopy /s ../ ../../$(TEAM_NAME)/repo
	rmdir /s /q ../../$(TEAM_NAME)/repo/src/doc
	cd ../../ && zip -r $(TEAM_NAME).zip $(TEAM_NAME)

help:
	@echo "Pro spuštění programu postupujte podle návodu: https://github.com/PremekJanda/IVS-projekt-2/blob/main/dokumentace.pdf"

profile:
	php profiling.php < ../profiling/10.txt
	php profiling.php < ../profiling/100.txt
	php profiling.php < ../profiling/1000.txt

doc: 
	doxygen Doxyfile