/*
Pebble uses https://github.com/phoboslab/Ejecta and appears to not support:
  XMLHttpRequest.responseXML
  new RegExp(string)
  new DOMParser()
  /regex/g.exec
*/

var MINUTE = 60000;

// Initialize config screen
var Clay = require('pebble-clay');
var clayConfig = require('./config');
new Clay(clayConfig);

function sendTrainTimes(from, soap) {
  console.log("Recieved soap: " + soap);
  
  var serviceToText = function(service) {
    var getTime = function(re, strLen) {
      var match = re.exec(service);
      if (!match) return '??';
      if (match[2] == 'On time') return match[1].slice(-strLen);
      return match[2].slice(-strLen);
    };
    var old = getTime(/<lt4:crs>OLD<[^<]+<lt4:st>([^<]+)<[^<]+<lt4:et>([^<]+)</, 2);
    var hhy = getTime(/<lt4:crs>HHY<[^<]+<lt4:st>([^<]+)<[^<]+<lt4:et>([^<]+)</, 2);
    var fpk = getTime(/<lt4:crs>FPK<[^<]+<lt4:st>([^<]+)<[^<]+<lt4:et>([^<]+)</, 5);
    var hfn, mog;
    if (from == 'HFN') {
      hfn = getTime(/<lt4:std>([^<]+)<[^<]+<lt4:etd>([^<]+)</, 5);
      mog = getTime(/<lt4:crs>MOG<[^<]+<lt4:st>([^<]+)<[^<]+<lt4:et>([^<]+)</, 2);
      return [hfn,fpk,hhy,old,mog].join(' ');
    } else {
      mog = getTime(/<lt4:std>([^<]+)<[^<]+<lt4:etd>([^<]+)</, 5);
      hfn = getTime(/<lt4:crs>HFN<[^<]+<lt4:st>([^<]+)<[^<]+<lt4:et>([^<]+)</, 5);
      return [mog,old,hhy,fpk,hfn].join(' ');
    }
  };
    
  var message = {
    'MSG_TRAIN_TEXT': soap.split('</lt4:service><lt4:service>').map(serviceToText).join('\n')
  };

  console.log("Sending message: " + message['MSG_TRAIN_TEXT']);
  Pebble.sendAppMessage(message);
}

function getTrains(from, to) {
  console.log("Requesting trains from " + from + " to " + to); 
  var xhr = new XMLHttpRequest();
  xhr.onload = function() {
    sendTrainTimes(from, xhr.responseText);
  };
  xhr.open('POST', 'https://lite.realtime.nationalrail.co.uk/OpenLDBWS/ldb8.asmx');
  xhr.setRequestHeader("Content-Type", "text/xml");
  var xml = '<?xml version="1.0" encoding="utf-8"?>' +
    '<soapenv:Envelope xmlns:soapenv="http://schemas.xmlsoap.org/soap/envelope/" xmlns:typ="http://thalesgroup.com/RTTI/2013-11-28/Token/types" xmlns:ldb="http://thalesgroup.com/RTTI/2015-11-27/ldb/">' +
       '<soapenv:Header>' +
          '<typ:AccessToken>' +
             '<typ:TokenValue>a12a8f08-2df8-4d77-b9cd-510f6d811419</typ:TokenValue>' +
          '</typ:AccessToken>' +
       '</soapenv:Header>' +
       '<soapenv:Body>' +
          '<ldb:GetDepBoardWithDetailsRequest>' +
             '<ldb:numRows>3</ldb:numRows>' +
             '<ldb:crs>' + from + '</ldb:crs>' +
             '<ldb:filterCrs>' + to + '</ldb:filterCrs>' +
          '</ldb:GetDepBoardWithDetailsRequest>' +
       '</soapenv:Body>' +
    '</soapenv:Envelope>';
  xhr.send(xml);
}

function gotLocation(pos) {
  console.log("Got position: " + pos.coords.latitude); 
  if (pos.coords.latitude > 51.79) {  // Home
    getTrains('HFN', 'MOG');
  } else if (pos.coords.latitude > 51.57) {  // Travel
  } else {  // London
    getTrains('MOG', 'HFN');
  }
}

function sendUpdates(e) {
  console.log("Getting position"); 
  navigator.geolocation.getCurrentPosition(gotLocation, null, {enableHighAccuracy: false, maximumAge: 9 * MINUTE, timeout: MINUTE});
}

Pebble.addEventListener('ready', sendUpdates);
Pebble.addEventListener('appmessage', sendUpdates);