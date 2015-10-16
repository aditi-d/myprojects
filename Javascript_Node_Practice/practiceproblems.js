/*
This is a semi-famous algorithm known as the Fisher-Yates shuffle (sometimes called the Knuth shuffle).
Question on: https://www.interviewcake.com/
*/

var arr = [1,2,3,4,5,6,7,8,9];

function inPlaceShuffle() {
	for (var i = 0; i < arr.length; i++) {
		var random_pos = get_random(i,8);
		//var random_pos2 = get_random(0,8);
		var temp = arr[random_pos];
		arr[random_pos] = arr[i];
		arr[i] = temp;
	}
}

function get_random(min, max) {
	var rand = Math.floor(Math.random() * (max - min + 1)) + min;
	console.log(rand);
	return rand;
}

console.log(arr);
inPlaceShuffle();
console.log(arr);