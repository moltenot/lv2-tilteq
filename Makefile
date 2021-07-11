BUNDLENAME=tilteq.lv2
INSTALLDIR=~/.lv2/

.PHONY: install bundle

tilteq.so:
	g++ -o tilteq.so  -shared -fPIC -DPIC tilteq.cpp `pkg-config --cflags --libs lv2-plugin`

clean:
	rm tilteq.so

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