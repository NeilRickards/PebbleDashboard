var MINUTE = 60000;

function sendTrainTimes(from, soap) {
  var message = {};

  var getTime = function(re) {
    var match = re.exec(soap);
    return match == null ? '??' : match[1];
  }
  var depart = getTime(/<lt4:std>([^<]+)</);
  var old = getTime(/<lt4:crs>OLD<[^<]+<lt4:st>\d+:([^<]+)</);
  var hhy = getTime(/<lt4:crs>HHY<[^<]+<lt4:st>\d+:([^<]+)</m);
  var fpk = getTime(/<lt4:crs>FPK<[^<]+<lt4:st>([^<]+)</m);
  if (from == 'MOG') {
    var hfn = getTime(/<lt4:crs>HFN<[^<]+<lt4:st>([^<]+)</m);
    message.KEY_TRAIN_TEXT = [depart,old,hhy,fpk,hfn].join(' ') + '\n' + [depart,old,hhy,fpk,hfn].join(' ');
  } else {
    var mor = getTime(/<lt4:crs>MOG<[^<]+<lt4:st>([^<]+)</m);
    message.KEY_TRAIN_TEXT = [depart,fpk,hhy,old,mor].join(' ') + '\n' + [depart,fpk,hhy,old,mor].join(' ');
  }

  Pebble.sendAppMessage(message);
}

function getTrains(from, to) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function() {
    sendTrainTimes(from, this.responseText);
  };
  xhr.open('POST', 'https://lite.realtime.nationalrail.co.uk/OpenLDBWS/ldb8.asmx');
  xhr.setRequestHeader("Content-Type", "text/xml");
  var xml = '<?xml version="1.0" encoding="utf-8"?>' +
    '<soapenv:Envelope xmlns:soapenv="http://schemas.xmlsoap.org/soap/envelope/" xmlns:typ="http://thalesgroup.com/RTTI/2013-11-28/Token/types" xmlns:ldb="http://thalesgroup.com/RTTI/2015-11-27/ldb/">' +
       '<soapenv:Header>' +
          '<typ:AccessToken>' +
             '<typ:TokenValue>####</typ:TokenValue>' +
          '</typ:AccessToken>' +
       '</soapenv:Header>' +
       '<soapenv:Body>' +
          '<ldb:GetFastestDeparturesWithDetailsRequest>' +
             '<ldb:crs>' + from + '</ldb:crs>' +
             '<ldb:filterList>' +
                '<ldb:crs>' + to + '</ldb:crs>' +
             '</ldb:filterList>' +
          '</ldb:GetFastestDeparturesWithDetailsRequest>' +
       '</soapenv:Body>' +
    '</soapenv:Envelope>';
  xhr.send(xml);
}

function gotLocation(pos) {
  if (pos.coords.latitude > 51.79) {  // Home
    getTrains('HFN', 'MOG');
  } else if (pos.coords.latitude > 51.57) {  // Travel
  } else {  // London
    getTrains('MOG', 'HFN');
  }
}

function sendUpdates(e) {
  navigator.geolocation.getCurrentPosition(gotLocation, null, {enableHighAccuracy: false, maximumAge: 9 * MINUTE, timeout: MINUTE});
}

Pebble.addEventListener('ready', sendUpdates);
Pebble.addEventListener('appmessage', sendUpdates);