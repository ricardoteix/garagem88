function travar(estado) {var aguarde = document.getElementById('aguarde'); aguarde.style.display = estado;}

function onSubmit() { travar('block'); } 

function getDistanceFromLatLonInMeters(lat1,lon1) {
	var lat2 = -8.09659;
	var lon2 = -34.916468;
	var R = 6371;
	var dLat = deg2rad(lat2-lat1);
	var dLon = deg2rad(lon2-lon1); 
	var a = Math.sin(dLat/2) * Math.sin(dLat/2) + Math.cos(deg2rad(lat1)) * Math.cos(deg2rad(lat2)) * Math.sin(dLon/2) * Math.sin(dLon/2); 
	var c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1-a)); 
	var d = R * c;
	return d * 1000;
}

function deg2rad(deg) { return deg * (Math.PI/180); }

function getLocation() { 
    if (navigator.geolocation) {
        navigator.geolocation.getCurrentPosition(onPosition);
    } else {
       alert("Não foi possível determinar sua localização.");
    }
}

function onPosition(position) {
    var dist = getDistanceFromLatLonInMeters(position.coords.latitude,position.coords.longitude);
    if (dist > 100) {
    	alert("Você está muito distante para controlar sua casa. " + dist.toFixed(2).split(".").join(",") + " metros.");
    } else {
    	travar('none');
    }
}