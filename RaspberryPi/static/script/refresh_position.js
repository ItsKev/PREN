function update_value(){
    $.ajax({
        url: "/pos",
        type: "get",
        success: function (respone) {
            var jsonObj = jQuery.parseJSON(respone);
            $("#x").html("X-Position: " + jsonObj.x);
            $("#z").html("Z-Position: " + jsonObj.z);
        },
        error: function (respone) {
            $("#error").html("Fehler: " + respone.responseText);
        }
    });
}