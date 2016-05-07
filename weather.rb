# Encoding: utf-8
require 'rubygems'
require 'bundler'

Bundler.require

require 'sinatra'
require 'mongoid'
require 'roar/json/hal'
require 'rack/conneg'

configure do
  Mongoid.load!("config/mongoid.yml", settings.environment)
  set :server, :puma # default to puma for performance
end

class Temp
  include Mongoid::Document
  include Mongoid::Timestamps

  field :temp, type: String
  field :pres, type: String
  field :alt, type: String
end

module TempRepresenter
  include Roar::JSON::HAL

  property :temp
  property :pres
  property :alt
  property :created_at, :writeable => false

  link :self do
    "/temps/#{id}"
  end
end

get '/temps/?' do
  temps = Temp.all.order_by(:created_at => 'desc')
  TempRepresenter.for_collection.prepare(temps).to_json
end

post '/temps/?' do
  temp = params[:temp]
  pres = params[:pres]
  alt = params[:alt]

  if temp.nil? or temp.empty?
    halt 400, {:message => "temp field cannot be empty"}.to_json
  end

  temp = Temp.new(:temp => temp, :pres => pres, :alt => alt)
  if temp.save
    [201, temp.extend(TempRepresenter).to_json]
  else
    [500, {:message => "Failed to save temp"}.to_json]
  end
end
