BUNDLENAME=tilteq.lv2
INSTALLDIR=~/.lv2/
UTILS_DIR=dsp-utils
FUNC_OB=$(UTILS_DIR)/functions.o

.PHONY: install bundle clean uninstall reinstall test ffttest

tilteq.so: $(FUNC_OB)
	g++ -o tilteq.so  -shared -fPIC -DPIC tilteq.cpp $(FUNC_OB) `pkg-config --cflags --libs lv2-plugin fftw3 fftw3f`

$(FUNC_OB):
	$(MAKE) -C dsp-utils

clean:
	rm tilteq.so fft_bin

bundle: tilteq.so
	mkdir -p 	$(BUNDLENAME)
	mv tilteq.so  $(BUNDLENAME)
	cp manifest.ttl tilteq.ttl $(BUNDLENAME)

install: bundle
	mv $(BUNDLENAME) $(INSTALLDIR)

uninstall:
	rm -rf $(INSTALLDIR)/$(BUNDLENAME)

reinstall: uninstall install

test:
	jalv https://github.com/moltenot/lv2-tilteq