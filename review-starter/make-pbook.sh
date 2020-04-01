#!/bin/bash

# replace images
mv images images-temp
mv images-gray images

# replace config
mv config.yml config-temp.yml
mv config-pbook.yml config.yml

# rake
gem install combine_pdf
rake pdf
rake pdf:nombre

# restore config
mv config.yml config-pbook.yml
mv config-temp.yml config.yml

# restore images
mv images images-gray
mv images-temp images
