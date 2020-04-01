#!/bin/bash

for INPUTFILE in `find ./images/ -type f`; do
	# ignore
	if [[ $INPUTFILE = */tag.png ]]; then continue; fi
	if [[ $INPUTFILE = *yuka-matsuba/oneday.png ]]; then continue; fi
	if [[ $INPUTFILE = *yuka-matsuba/oneshot.png ]]; then continue; fi
	if [[ $INPUTFILE = *yuka-matsuba/skills.png ]]; then continue; fi
	if [[ $INPUTFILE = *atsushi-umakatsu/architecture.png ]]; then continue; fi
	if [[ $INPUTFILE = *atsushi-umakatsu/bespoken-flow.png ]]; then continue; fi
	
	convert -type GrayScale -modulate 110 +contrast $INPUTFILE ${INPUTFILE/\/images\//\/images-gray\/}
done
