var Tree = function() {
	this._root = null;
};

var TreeNode = function(data) {
	this._data = data;
	this._left = null;
	this._right = null;
};

Tree.prototype.compare = function(data1, data2) {
	if (data1 === data2)
		return 0;
	else if (data1 < data2)
		return -1;
	else
		return 1;
}


Tree.prototype.createNode = function(data) {
	console.log("adding::" + data);
	if (!this._root) {
		this._root = new TreeNode();
		this._root._data = data;
		//console.log(this._data);
	}
	else {
		var currentNode = this._root;
		//var parentNode = currentNode;
		var newNode = new TreeNode(data);
		newNode._data = data;
		while(currentNode != null) {
			parentNode = currentNode;
			if (this.compare(newNode._data, currentNode._data) <= 0) {
				//new becomes left of current
				currentNode = currentNode._left;
			}
			else {
				currentNode = currentNode._right;
			}
		}
		if (this.compare(newNode._data, parentNode._data) <= 0) {
			//console.log("adding ");
			parentNode._left = newNode;
		}
		else {
			parentNode._right = newNode;
		}
	}
}


Tree.prototype.displayTree = function() {
	printTree(this._root);
	function printTree(node) {
		//console.log(node._data);
		if (node != null) {
			printTree(node._left);
			console.log(node._data);
			printTree(node._right);
		}
	}
}

Tree.prototype.displayTreePre = function() {
	printTree(this._root);
	function printTree(node) {
		//console.log(node._data);
		if (node != null) {
			console.log(node._data);
			printTree(node._left);
			printTree(node._right);
		}
	}
}

Tree.prototype.displayTreePost = function() {
	printTree(this._root);
	function printTree(node) {
		//console.log(node._data);
		if (node != null) {
			printTree(node._left);
			printTree(node._right);
			console.log(node._data);
		}
	}
}

Tree.prototype.height = function(node) {
	if (node == null) {
		return 0;
	}
	else {
		var lheight = this.height(node._left);
		var rheight = this.height(node._right);
		if (rheight > lheight) {
			return rheight + 1;
		}
		else {
			return lheight + 1;
		}
	}
}

Tree.prototype.isBalanced = function() {
	if (this._root == null) {
		return true;
	}
	else {
		var lheight = this.height(this._root._left);
		var rheight = this.height(this._root._right);

		console.log("lheight::" + lheight + " rheight::" + rheight);

		if (Math.abs(lheight - rheight) <= 1)
			return true;
		return false;
	}
}

Tree.prototype.isBalancedMethodTwo = function(node) {
	if (node == null) {
		return 0;
	}
	else {
		var lheight = this.isBalancedMethodTwo(node._left);
		var rheight = this.isBalancedMethodTwo(node._right);

		console.log("lheight::" + lheight + " rheight::" + rheight);

		var diff = lheight - rheight;
		if (Math.abs(diff) > 1)
			return -1;
		else {
			return Math.max(lheight, rheight) + 1;
		}
	}
}

Tree.prototype.createBalancedTree = function(arr, start, end) {
	if (start <= end) {
			//return 0;
		var mid = Math.floor((start + end)/2);
		var node = arr[mid];
		this.createNode(node);
		this.createBalancedTree(arr, start, mid - 1);
		this.createBalancedTree(arr, mid + 1, end);
	}
}

Tree.prototype.displayLevelwise = function() {
	var arr = [];
	//arr[0] = [];
	var temparr = [];
	temparr.push(this._root);
	arr.push(temparr);
	//arr[0].push(root);
	var size = temparr.length;
	var level = 0;
	while(size > 0) {
		var temp = [];
		for (var i = 0; i < size; i++) {
			var left = arr[level][i]._left;
			var right = arr[level][i]._right;
			if (left != null) {
				temp.push(left);
			}
			if (right != null) {
				temp.push(right);
			}
		}
		size = temp.length;
		level++;
		if (size > 0) {
			arr.push(temp);
		}
	}
	return arr;
}

/*Tree.prototype.displayLevelWiseRec = function() {
	var arr = [];
	//arr[0] = [];
	var temparr = [];
	arr.push(temparr);
	//arr[0].push(root);
	var size = temparr.length;
	var level = 0;
	while(size > 0) {
		for (var i = 0; i < size; i++) {
			var left = arr[level][i]._left;
			var right = arr[level][i].right;
			if (left != null) {
				arr[level + 1].push(left);
			}
			if (right != null) {
				arr[level + 1].push(right);
			}
		}
		size = arr[level + 1].length;
	}
	return arr;
}*/

var last_printed = Number.MIN_SAFE_INTEGER;
Tree.prototype.checkBST = function(node) {
	if (node == null)
		return true;

	if (!this.checkBST(node._left)) return false;

	if (node != null && node._data >= last_printed) {
		last_printed = node._data;
		return true;	
	}

	if (!this.checkBST(node._right)) return false;

	return true;
}

var tree = new Tree();
tree.createNode(6);
tree.createNode(5);
tree.createNode(9);
tree.createNode(7);
tree.createNode(8);
tree.createNode(13);
tree.displayTree();
console.log("======================");
tree.displayTreePre();
console.log("======================");
tree.displayTreePost();
console.log(tree.isBalanced());
console.log(tree._root);
console.log(tree.isBalancedMethodTwo(tree._root) == -1 ? false : true);

var tree2 = new Tree();
var arr = new Array(5,6,7,8,9,13,15);
tree2.createBalancedTree(arr,0,arr.length-1);
var levelWiseList = tree.displayLevelwise();
levelWiseList.forEach(function (level) {
	console.log(level);
});
console.log(tree.checkBST(tree._root));
