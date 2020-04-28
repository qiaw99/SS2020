function getInput(){
    var text = document.getElementById("feld").value;
    if(text == ""){
        document.getElementById("bereich").innerHTML="Hallo Unbekannter!";
    }else{
        document.getElementById("bereich").innerHTML="Hallo " + text + " !";
    }
}
