function init(){
  getLocation();
  setupCamera();
  setupHammer('origin-input', 'doubletap', 'press');
  setupHammer('destination-input', 'doubletap', 'press');
}

var myPos;
var zoom = 13;

var LatLng = {
  lat: 58.39,
  lng: 13.845
};

var map = new google.maps.Map(document.getElementById('map'), {
  mapTypeControl: false,
  center: LatLng,
  zoom: zoom,
  gestureHandling: 'greddy',
  disableDefaultUI: true
});




/* SEARCH BOXES */
let searchOptions = {
  types: []
};

const originInput = document.getElementById('origin-input');
const autocomplete1 = new google.maps.places.Autocomplete(originInput, searchOptions);

const destinationInput = document.getElementById('destination-input');
const autocomplete2 = new google.maps.places.Autocomplete(destinationInput, searchOptions);

const startRoute = document.getElementById('startRoute');
const showRoute = document.getElementById('showRoute');

const directionsContainer = document.getElementById('directions')
const navigation = document.getElementById('navigation');

const output = document.getElementById('output');
const cancel = document.getElementById('cancelRoute');

const travelModes = document.getElementById('mode-selector');

var firstStep;


/* Requests */
var directionsService = new google.maps.DirectionsService();
var directionsDisplay = new google.maps.DirectionsRenderer();
var geocoder = new google.maps.Geocoder();

 /* Arrow */
var userMarker = new google.maps.Marker({
  icon: {
  path: google.maps.SymbolPath.FORWARD_CLOSED_ARROW,
  scale: 2,
  strokeColor: 'blue',
  rotation: 90
},
map: map,
});

/* EVENTS */
function geocodeAddress(geocoder, resultsMap) {
        geocoder.geocode({'address': originInput.value}, function(results, status) {
          if (status === 'OK') {
            resultsMap.setZoom(15);
            resultsMap.setCenter(results[0].geometry.location);
          } else {
            alert('Geocode was not successful for the following reason: ' + status);
          }
        });
      }

function getTravelType() {
  let checkbox = document.getElementsByClassName('travel-type');
  for (let i = 0; i < 3; i++) {
    if (checkbox[i].checked === true) {
      return checkbox[i].value;
    }
  }
}

function CalculateAndDisplayRoute() {
  setTimeout(function(){
    let steps = document.getElementById('steps');
    let distance = document.getElementById('distance');
    let time = document.getElementById('time');
    const travelType = getTravelType();

    //Set directionsDisplay for the map each time the function is run.
    directionsDisplay.setMap(map);

    if(originInput.value == ""){
      let posString = JSON.stringify(myPos);
      posString = convertString(posString);
      originInput.value = posString;

    }

    let request = {
      origin: originInput.value,
      destination: destinationInput.value,
      travelMode: google.maps.TravelMode[travelType], //Driving, Bicycling, Walking
      unitSystem: google.maps.UnitSystem.METRIC
    };

    directionsService.route(request, function (result, status) {
      if (status == google.maps.DirectionsStatus.OK)
      {
        directionsDisplay.setDirections(result);
        output.style.visibility = "visible";
        output.style.display = "block";
        time.innerHTML = "("+ result.routes[0].legs[0].duration.text + ")";
        distance.innerHTML = result.routes[0].legs[0].distance.text;

        travelModes.style.display = "none";
        showRoute.style.display = "none";
        startRoute.style.display = "block";
        cancel.style.display = "block";

        firstStep = result.routes[0].legs[0].steps[0];
      }
      else
      {
        directionsDisplay.setDirections({ routes: [] });
      }
    });
  }, 400);
}

function panTo(){
  map.setZoom(15);
  geocodeAddress(geocoder, map);
}

function intializeRoute(){
  setTimeout(function(){
    panTo();
    let timeWaypoint = document.getElementById('waypoint-time');
    let streetWaypoint = document.getElementById('street-waypoint');

    navigation.style.display = "none";
    directionsContainer.style.display = "block";
    timeWaypoint.innerHTML = firstStep.duration.text;
    streetWaypoint.innerHTML = firstStep.distance.text;

    startRoute.style.display = "none";

    userMarker.setPosition(myPos);
  }, 400);

}

function getLocation(){
  if(navigator.geolocation){
    navigator.geolocation.getCurrentPosition(function(position){
      myPos = {
        lat: position.coords.latitude,
        lng: position.coords.longitude
      };
    });
  }
}

function convertString(posString){
  posString = posString.replace(/[\])}[{('"]/g, '');
  posString = posString.replace(/:\s*/g, '');

  posString = posString.replace("lat", '');
  posString = posString.replace("lng", '');

  return posString;
}

function cancelRoute(){
  setTimeout(function(){
    destinationInput.value = "";

    directionsDisplay.setMap(null);
    map.setZoom(13);

    cancel.style.display = "none";
    showRoute.style.display = "block";
    travelModes.style.display = "flex";
    startRoute.style.display = "none";
    directionsContainer.style.display = "none"
    output.style.display = "none";
    navigation.style.display = "flex";
  }, 400);
}

/************* Shaker.js ******************/


shaker.on('shake', function(e){
  if(e.shakes > 5){
    let form = document.getElementById('navigation-form');
    form.reset();
  }
});

const landscapeOutput = document.getElementById('l-output-city');
const landscapeOutputTime = document.getElementById('l-output-time');

shaker.on('tilt', function (e) {
  let angle = e.alpha;
  let angleRounded = Math.round(angle);
  rotateMarker(angle);

  if(angle < 90){
    landscapeOutput.innerHTML = 'Malmö';
    landscapeOutputTime.innerHTML = '4h 10min';
  }
  else if (angle > 90 && angle < 180)
  {
    landscapeOutput.innerHTML = 'Göteborg';
    landscapeOutputTime.innerHTML = '1h 58min';
  }
  else if(angle > 180 && angle < 270)
  {
    landscapeOutput.innerHTML = 'Stockholm';
    landscapeOutputTime.innerHTML = '3h 53min';
  }
  else if(angle > 270)
  {
    landscapeOutput.innerHTML = 'Uppsala';
    landscapeOutputTime.innerHTML = '3h 43min';
  }
});


function rotateMarker(deg){
  let icon = userMarker.getIcon();
  icon.rotation = deg;

  userMarker.setIcon(icon);
}

/************* Hammer.js ******************/

function setupHammer(input, gesture1, gesture2) {
  var input = document.getElementById(input);
  let hammer = new Hammer(input).on(gesture1, function(e){
    input.value = '';
  });

  let hammerTime = new Hammer(input).on(gesture2, function(e){
    input.select();
  });
}



/************* Camera API ******************/
let front = false;
const constraints = {
  video: { facingMode: (front? "user" : "environment") }
};

const video = document.querySelector('video');

 function setupCamera(){

    if (navigator.mediaDevices === undefined)
    {
      navigator.mediaDevices = {};
    }

    if (navigator.mediaDevices.getUserMedia === undefined) {

      navigator.mediaDevices.getUserMedia = function(constraints) {

      var getUserMedia = (navigator.getUserMedia ||
                          navigator.webkitGetUserMedia ||
                          navigator.mozGetUserMedia);

      if (!getUserMedia)
      {
        return Promise.reject(new Error('getUserMedia is not implemented in this browser'));
      }

      return new Promise(function(resolve, reject) {
        getUserMedia.call(navigator, constraints, resolve, reject);
      });

      }
    }

    navigator.mediaDevices.getUserMedia(constraints)
    .then(function(stream) {

      // Older browsers may not have srcObject

      video.src = window.URL.createObjectURL(stream);

      localMediaStream = stream;

      video.onloadedmetadata = function(e) {

        video.play();

      };
    })

    .catch(function(err) {

      console.log(err.name + ": " + err.message);
    });

  }
