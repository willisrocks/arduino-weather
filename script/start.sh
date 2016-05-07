#!/bin/bash

bundle check || bundle install
bundle exec ruby weather.rb
