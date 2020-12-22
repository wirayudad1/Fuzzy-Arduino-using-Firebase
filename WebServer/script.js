// Initialize Firebase
//The project has been deleted in firebase
//fill in your own config info 
var config = {
    apiKey: "AIzaSyDF6qYfCMsIzEF7R5_FhzIbSLZtlVOv0fk",
    authDomain: "webserver-722a2.firebaseapp.com",
    databaseURL: "https://webserver-722a2.firebaseio.com",
    projectId: "webserver-722a2",
    storageBucket: "webserver-722a2.appspot.com",
    messagingSenderId: "410531293796",
    appId: "1:410531293796:web:d00bb25ab86ffd4eebcd71",
    measurementId: "G-09MKW3PVHX"
  };
  firebase.initializeApp(config);

  
  $(document).ready(function(){
    var database = firebase.database();
    var LED_STATUS;
  
    database.ref().on("value", function(snap){
      LED_STATUS = snap.val().LED_STATUS;
      if(LED_STATUS == 1){
        $(".lightStatus").text("ON");
        document.body.style.fontColor = "red";
      } else {
        $(".lightStatus").text("OFF");
        document.body.style.fontColor = "green";
      }
    });
  
    $(".lightButton").click(function(){
      var firebaseRef = firebase.database().ref().child("LED_STATUS");
  
      if(LED_STATUS == 1){
        firebaseRef.set(0);
        LED_STATUS = 0;
      } else {
        firebaseRef.set(1);
        LED_STATUS = 1;
      }
    });

    var ref = firebase.database().ref().child("dht");
    ref.on ("value", function(snapshot){
    console.log(snapshot.val()) ; 
    var temp = snapshot.val().temperature;
    document.getElementById("temp").innerHTML=temp;
    })

    var ref = firebase.database().ref().child("dht");
    ref.on ("value", function(snapshot){
    console.log(snapshot.val()) ; 
    var hum = snapshot.val().kelembaban;
    document.getElementById("hum").innerHTML=hum;
    })

    var ref = firebase.database().ref().child("dht");
    ref.on ("value", function(snapshot){
    console.log(snapshot.val()) ; 
    var kondisi = snapshot.val().kondisi;
    document.getElementById("kondisi").innerHTML=kondisi;
    })
    

  });
  