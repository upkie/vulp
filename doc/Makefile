# Makefile for Sphinx documentation
#
# Copyright 2022 Stéphane Caron

# You can also set these variables from the command line.
DOXYGEN      = doxygen
HTMLDIR      = html
REMOTEDIR    = tasts-robots.org:public_html/doc/vulp

.PHONY: clean html open upload

html:
	(cd .. && $(DOXYGEN) Doxyfile)

clean:
	rm -rf $(HTMLDIR)

open:
	firefox $(HTMLDIR)/index.html

images/%.eps : figures/%.dot
	dot $< -Teps -o $@

images/%.png : figures/%.dot
	dot $< -Tpng -o $@

images/%.svg : figures/%.dot
	dot $< -Tsvg -o $@

figures: images/state-machine.eps images/state-machine.svg

upload: clean html
	rsync -auvz --delete-after $(HTMLDIR)/ $(REMOTEDIR)/
