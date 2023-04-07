// Rest based chat client
// Jim Skon 2022
// Kenyon College

var baseUrl = 'http://34.229.136.9:5005';
var state="off";
var myname="";
var inthandle;
var usersLoggedIn;

/* Start with text input and status hidden */
document.getElementById('chatinput').style.display = 'none';
document.getElementById('status').style.display = 'none';
document.getElementById('leave').style.display = 'none';
// // Action if they push the join button
document.getElementById('login-btn').addEventListener("click", (e) => {
	join();
})

/* Set up buttons */
document.getElementById('leave-btn').addEventListener("click", leaveSession);
document.getElementById('send-btn').addEventListener("click", sendText);
document.getElementById('save-changes').addEventListener("click", register);

// Watch for enter on message box
document.getElementById('message').addEventListener("keydown", (e)=> {
    if (e.code == "Enter") {
		sendText();
    }   
});


// Call function on page exit
window.onbeforeunload = leaveSession;

function completeRegister(results) {
	var status = results['status'];
	if (status == "name taken") {
		alert("Failed to Register User: username taken");
		return;
	}
	else if (status == "email taken") {
		alert("Failed to Register User: email taken");
		return;
	}
	else if (status == "password short") {
		alert("Failed to Register User: password too short");
		return;
	}
	var user = results['user'];
	console.log("Register:"+user);
}

function register() {
	myname = document.getElementById('orangeForm-name').value;
	mypass = document.getElementById('orangeForm-pass').value;
	myemail = document.getElementById('orangeForm-email').value;
	fetch(baseUrl+'/chat/register/'+ myname + '/' + mypass + '/' + myemail, {
		method: 'get'
	})
	.then (response => response.json() )
	.then (data => completeRegister(data))
	.catch(error => {
		{alert("Error: Something went wrong:"+error);}
	})
}


function completeJoin(results) {
	var status = results['status'];
	if (status != "success") {
		alert("Username or Password Incorrect!");
		leaveSession();
		return;
	}
	var user = results['user'];
	console.log("Join:"+user);
	startSession(user);
}

function join() {
	myname = document.getElementById('yourname').value;
	mypass = document.getElementById('yourpass').value;
	fetch(baseUrl+'/chat/join/'+myname+'/'+mypass, {
        method: 'get'
    })
    .then (response => response.json() )
    .then (data =>completeJoin(data))
    .catch(error => {
        {alert("Error: Something went wrong:"+error);}
    })
}

function completeSend(results) {
	var status = results['status'];
	if (status == "success") {
		console.log("Send succeeded");
		document.getElementById('message').value = "";
	} else {
		alert("Error sending message!");
	}
}

//function called on submit or enter on text input
function sendText() {
    var message = document.getElementById('message').value;
    console.log("Send: "+myname+":"+message);
	document.getElementById('message').value = "";
	fetch(baseUrl+'/chat/send/'+myname+'/'+message, {
        method: 'get'
    })
    .then (response => response.json() )
    .then (data =>completeSend(data))
    .catch(error => {
        {alert("Error: Something went wrong:"+error);}
    })    

}

function completeFetch(result) {
	messages = result["messages"];
	messages.forEach(function (m,i) {
		name = m['user'];
		message = m['message'];
		document.getElementById('chatBox').innerHTML +=
	    	"<font color='red'>" + name + ": </font>" + message + "<br />";
	});
}

/* Check for new messaged */
function fetchMessage() {
	fetch(baseUrl+'/chat/fetch/'+myname, {
        method: 'get'
    })
    .then (response => response.json() )
    .then (data =>completeFetch(data))
    .catch(error => {
        {console.log("Server appears down");}
    })  	
}


function completeGetUsers(result) {
	users = result["onlineUsers"];
	document.getElementById('onlineUsers').innerHTML="";
	users.forEach(function(u, i) {
		thisUser = u;
		document.getElementById('onlineUsers').innerHTML+=
		"<li>" + thisUser + "</li>";
	});
}

function getUsers() {
	fetch(baseUrl+'/chat/list', {
		method: 'get'
	})
	.then (response => response.json() )
    .then (data =>completeGetUsers(data))
    .catch(error => {
        {console.log("Something went wrong:"+error);}
    })  
}

function leave() {
	console.log(myname + " is the user being passed to /chat/leave/");
	fetch(baseUrl+'/chat/leave/'+myname, {
		method: 'get'
	})
	.then (console.log("leave request ran"))
	.catch(error => {
		{console.log("Something went wrong:" + error);}
	})
}


/* Functions to set up visibility of sections of the display */
function startSession(name){
    state="on";
    
    document.getElementById('yourname').value = "";
    document.getElementById('register').style.display = 'none';
    document.getElementById('user').innerHTML = "User: " + name;
    document.getElementById('chatinput').style.display = 'block';
    document.getElementById('status').style.display = 'block';
    document.getElementById('leave').style.display = 'block';
    /* Check for messages every 500 ms */
    inthandle=setInterval(fetchMessage,500);
    usersLoggedIn=setInterval(getUsers,2000);
}

function leaveSession(){
	console.log("leaving session...");
    state="off";
    leave();
    document.getElementById('yourname').value = "";
    document.getElementById('register').style.display = 'block';
    document.getElementById('user').innerHTML = "";
    document.getElementById('chatinput').style.display = 'none';
    document.getElementById('status').style.display = 'none';
    document.getElementById('leave').style.display = 'none';
	clearInterval(inthandle);
	clearInterval(usersLoggedIn);
}


