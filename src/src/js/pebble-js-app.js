var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function getFeed() {
  // Construct URL
  var url = "http://jsonip.com/";

  // Send request to IP Address
  xhrRequest(url, 'GET',
    function(responseText) {
      var json = JSON.parse(responseText);

      // Send to Pebble
      Pebble.sendAppMessage({'apidata':json.ip},
        function(e) {
          console.log("Date sent to Pebble!");
        },
        function(e) {
          console.log("Error sending data to Pebble!");
        }
      );
    }      
  );
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log("PebbleKit JS ready!");
    getFeed();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log("AppMessage received!");
    getFeed();
  }                     
);
