
function homeButton()
{
    var div = document.getElementById("home_button");
    div.style.left = 100;
    div.style.top = 100;
}

function body()
{
    var body = document.getElementById("body")
    body.style.left = 100;
    body.style.top = 100;
}

function buttonSwitch(buttonName)
{
    document.getElementById("home_button").className = "btn btn-outline-dark";
    document.getElementById("project_button").className = "btn btn-outline-dark";
    document.getElementById("photography_button").className = "btn btn-outline-dark";
    document.getElementById("resume_button").className = "btn btn-outline-dark";

    document.getElementById(buttonName).className = "btn btn-dark";
}