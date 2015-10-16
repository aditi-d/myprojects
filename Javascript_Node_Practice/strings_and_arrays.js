/* Based on interview questions in cracking the coding interview questions*/

/* ================================================================================================== */
/* This function replaces all the extra chars to have a string with only unique chars*/
/* TODO */
function getUniqueChars(str) {
	var k = 0;
	for (var i = 0; i < str.length; i++) {
		for (var j = 0; j < i; j++) {
			if (str[i] === str[j]) {
				console.log(i + ' ' + j + ' ' + k);
				k++;
			}
		}
		//var temp = str.charAt(k);
		if (k <= i) {
			str[i] = str[k];
			k++;
		}
	}
	return str.slice(0,i-1);
}

var temp = ['a','b','c','d','a','e','b'];
console.log(temp);
console.log(getUniqueChars(temp));

/* ================================================================================================== */

/* This function detects if a string contains unique characters.
It returns true if the string has unique chars, false otherwise.
*/
function detectUniqueChars(str) {
	var comparer = 'a';
	var bitArray = 0;
	for(var i = 0; i < str.length; i++) {
		var pos = str.charCodeAt(i) - comparer.charCodeAt(0);
		var tempBit = 1 << pos;
		var tempComparer = tempBit & bitArray;
		var chk = tempComparer >> pos;
		if (chk === 0) {
			//first occurance
			bitArray = bitArray | pos;
		}
		else
			return false;
	}
	return true;
}

var testUniqueString = "abcdef";
console.log(testUniqueString + "::" + detectUniqueChars(testUniqueString));

/* ================================================================================================== */
/*Test if a string is a permutation of the other */
function detectPermuation(origString, permutedString) {
	var origBitArray = 0;
	var comparer = 'a';
	var permutedStringBitArray = 0;
	for (var i = 0; i < origString.length; i++) {
		var pos = origString.charCodeAt(i) - comparer.charCodeAt(0);
		var tempBit = 1 << pos;
		origBitArray = origBitArray | tempBit;
	}
	for (var i = 0; i < permutedString.length; i++) {
		var pos = permutedString.charCodeAt(i) - comparer.charCodeAt(0);
		var tempBit = 1 << pos;
		permutedStringBitArray = permutedStringBitArray | tempBit;
	}
	console.log(origString + " " + permutedString + " " + permutedStringBitArray + "::" + origBitArray);
	if ((permutedStringBitArray | origBitArray) === origBitArray) {
		//console.log(permutedStringBitArray + "::" + origBitArray);
		return true;
	}
	return false;
}

var testPermutedString = "deaf";
var testPermutedString2 = "deafg";
console.log("Is " + testPermutedString + " permutation of " + testUniqueString + "::" + detectPermuation(testUniqueString,testPermutedString));
console.log("Is " + testPermutedString2 + " permutation of " + testUniqueString + "::" + detectPermuation(testUniqueString,testPermutedString2));

/* ================================================================================================== */

/* Function to replace all the spaces in a string by %20 */

function replaceSpaces(str) {
	var spaces = 0;
	if (str !== null  && typeof str === 'object') {
		for (var i = 0; i < str.length; i++) {
			if(str[i] === ' ') {
				//console.log("space");
				spaces++;
			}
		}
		var newLength = (Number(spaces) * 2) + Number(str.length);
		console.log(Number(spaces));
		for (var j = newLength - 1, i = str.length - 1; j >= 0; i--) {
			if (str[i] === ' ') {
				str[j] = '0';
				str[j-1] = '2';
				str[j-2] = '%';
				j -= 3;
			}
			else {
				str[j] = str[i];
				j--;
			}
			//console.log(i + "::" + j);
		}
	}
	return str;
}

var strWithSpaces = ['m','o','o','n',' ','l','i','g','h','t',' ','a','n','d',' ','s','t','a','r','s'];
console.log("String with spaces::" + strWithSpaces);
console.log("After replacing spaces::" + replaceSpaces(strWithSpaces));
console.log("Orig string::" + strWithSpaces);
//console.log(strWithSpaces[100]);
//console.log(Number.MAX_SAFE_INTEGER);

/* ================================================================================================== */
/* Function to rotate matrix in place by 90 degrees*/
function rotateInPlace(matrix) {
	var n = matrix.length;
	for (var layer = 0; layer < n/2; layer++) {
		for (var j = layer; j < n-1-layer; j++) { // for (var j = layer; j < n-layer; j++) -- to rotate by 180 degrees
			element = (n-1)-j;

			//save top
			var temp = matrix[layer][j];
			
			//replace top by left	
			matrix[layer][j] = matrix[element][layer];

			//replace left by bottom
			//console.log(n-layer);
			matrix[element][layer] = matrix[(n-1)-layer][element];

			//replace bottom by right
			matrix[(n-1)-layer][element] = matrix[(n-1)-element][(n-1)-layer];

			//replace right by top (temp)
			matrix[(n-1)-element][(n-1)-layer] = temp;
		}
	}
}

function displayMatrix(matrix) {
	var n = matrix.length;
	for (var i = 0; i < n; i++) {
		var line = '';
		if (matrix[j] !== 'undefined') {
			console.log(matrix[i]);
			/*for(var j = 0; j < matrix[i].length; j++) {
				line = matrix[i][j] + ' ';
			}*/
		}
		//console.log(line);
	}
}

var mat = new Array(6);
var count = 0;
for(var i = 0; i < 6; i++) {
	mat[i] = new Array(6);
	for (var j = 0; j < 6; j++) {
		mat[i][j] = count;
		count++;
	}
}
//console.log(mat);
displayMatrix(mat);
rotateInPlace(mat);
console.log("After rotation::");
displayMatrix(mat);

/* ================================================================================================== */
/* Function to set the row column to zero if one element is zero*/
function setRowColToZero(matrix) {
	var n = matrix.length;
	var row = new Array(matrix.length) , col = new Array(matrix[0].length);
	for (var i = 0; i < n; i++) { 
		for (var j = 0; j < matrix[i].length; j++) {
			if (matrix[i][j] === 0) {
				row[i] = true;
				col[i] = true;
			}
		}
	}

	for (var i = 0; i < n; i++) { 
		for (var j = 0; j < matrix[i].length; j++) {
			if (row[i] === true || col[j] === true) {
				matrix[i][j] = 0;
			}
		}
	}
}

var testmat = new Array(6);
var count = 0;
for(var i = 0; i < 6; i++) {
	testmat[i] = new Array(6);
	for (var j = 0; j < 6; j++) {
		testmat[i][j] = count;
		count++;
	}
}

displayMatrix(testmat);
setRowColToZero(testmat);
displayMatrix(testmat);

/* ================================================================================================== */
function detectRotation(str1, str2) {
	if (str1.length !== 0 && str1.length === str2.length) {
		testStr = str1.concat(str1);
		console.log(testStr);
		/*if (testStr.includes(str1)) {
			return true;
		}*/
		for (var i = 0; i < str2.length; i++) {
			var subs = testStr.substr(i, str2.length);
			if (subs === str2) {
				return true;
			}
		}
	}
	return false;
}

var str1 = "waterbottle";
var str2 = "bottlewater";
var str3 = "waterttlebo";
console.log("is " + str2 + " rotation of " + str1 + detectRotation(str1,str2));
console.log("is " + str3 + " rotation of " + str1 + detectRotation(str1,str3));
