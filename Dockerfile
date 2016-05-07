FROM ruby:2.2

RUN apt-get update -qq && apt-get install -y build-essential

ENV APP_HOME /app  
RUN mkdir $APP_HOME  
WORKDIR $APP_HOME

ENV BUNDLE_PATH /box

ADD . $APP_HOME
