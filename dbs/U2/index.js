function click(){
    var text = document.getElementById("feld").value;
    if(text == null){
        document.getElementById("bereich").innerHTML="Hallo Unbekannter!";
    }else{
        document.getElementById("bereich").innerHTML="Hallo " + text + " !";
    }
}
