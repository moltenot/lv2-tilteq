BUNDLENAME=tilteq.lv2
INSTALLDIR=~/.lv2/

.PHONY: install bundle clean uninstall reinstall test ffttest

tilteq.so:
	g++ -o tilteq.so  -shared -fPIC -DPIC -lfftw3 -lfftw3f -lm tilteq.cpp `pkg-config --cflags --libs lv2-plugin`

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

ffttest: fft_bin
	./fft_bin

fft_bin:
	gcc fft_test.c  -lfftw3 -Wall -lm -o fft_bin