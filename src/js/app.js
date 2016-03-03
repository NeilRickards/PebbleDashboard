function getTrains() {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    console.log(this.responseText);
  };
  xhr.open('POST', 'http://www.webservicex.net/stockquote.asmx?op=GetQuote');
  xhr.setRequestHeader("SOAPAction", "http://www.webserviceX.NET/GetQuote");
  xhr.setRequestHeader("Content-Type", "text/xml");
  var xml = '<?xml version="1.0" encoding="utf-8"?>' +
   '<soap:Envelope xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" ' +
                  'xmlns:xsd="http://www.w3.org/2001/XMLSchema" ' +
                  'xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/">' + 
     '<soap:Body> ' +
       '<GetQuote xmlns="http://www.webserviceX.NET/"> ' +
         '<symbol>MSFT</symbol> ' +
       '</GetQuote> ' +
     '</soap:Body> ' +
   '</soap:Envelope>';
  xhr.send(xml);
}

function gotLocation(pos) {
  getTrains();
  var message = {};
  if (pos.coords.latitude > 51.79) {  // Home
    message.KEY_TRAIN_TEXT = 'Home';
  } else if (pos.coords.latitude > 51.57) {  // Travel
    message.KEY_TRAIN_TEXT = 'Travel';
  } else {  // London
    message.KEY_TRAIN_TEXT = 'London';
  }
  Pebble.sendAppMessage(message);
}

function sendUpdates(e) {
  navigator.geolocation.getCurrentPosition(gotLocation, null, {enableHighAccuracy: false, maximumAge: 60000, timeout: 60000});
}

Pebble.addEventListener('ready', sendUpdates);
Pebble.addEventListener('appmessage', sendUpdates);