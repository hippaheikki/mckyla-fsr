function increment(amt, direction) {
	document.getElementsByName(direction+"_pressure")[0].value = parseInt(document.getElementsByName(direction+"_pressure")[0].value) + amt;
	
	if (document.getElementsByName(direction+"_pressure")[0].value < 100 && document.getElementsByName(direction+"_pressure")[0].value > 9) {
		document.getElementsByName(direction+"_pressure")[0].value = "0"+document.getElementsByName(direction+"_pressure")[0].value.toString();
	} else if (document.getElementsByName(direction+"_pressure")[0].value < 10 && document.getElementsByName(direction+"_pressure")[0].value > 0) {
		document.getElementsByName(direction+"_pressure")[0].value = "00"+document.getElementsByName(direction+"_pressure")[0].value.toString();
	} else if (document.getElementsByName(direction+"_pressure")[0].value < 1) {
		document.getElementsByName(direction+"_pressure")[0].value = "001";
	}
}

function arrowDown (toggle, direction) {
	if (toggle == 0) {
		document.getElementById("pad").src = "../img/dance.png";
	} else {
		document.getElementById("pad").src = "../img/dance"+direction+".png";	
	}
}