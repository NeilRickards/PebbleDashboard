function gotLocation(pos) {
//  console.log('{' + pos.coords.longitude + ',' + pos.coords.latitude + '}');
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