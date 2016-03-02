function sendUpdates(e) {
  navigator.geolocation.getCurrentPosition(function(pos) {
    var message = {};
    if (pos.coords.latitude > 51.79) {  // Home
      message.KEY_TRAIN_TEXT = 'Home';
    } else if (pos.coords.latitude > 51.57) {  // Travel
      message.KEY_TRAIN_TEXT = 'Travel';
    } else {  // London
      message.KEY_TRAIN_TEXT = 'London';
    }
    Pebble.sendAppMessage(message);
  });
}

Pebble.addEventListener('ready', sendUpdates);
Pebble.addEventListener('appmessage', sendUpdates);