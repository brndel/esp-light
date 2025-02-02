#include <Arduino.h>

// This file has the content of 'index.html' as a C-String

const char MAIN_page[] PROGMEM = R"--(

<!DOCTYPE html>
<html lang="de">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Light</title>
    <link rel="icon" href="favicon.svg">
    <style>
        html {
            font-family: Arial, Helvetica, sans-serif;
            background-color: var(--background-color);
            color: white;
            display: flex;
            align-items: center;
            justify-content: center;
            height: 100%;
            width: 100%;

            --background-color: rgb(40, 40, 40);
            --surface-color: rgb(30, 30, 30);

            --border-color: rgba(255, 255, 255, 0.5);
            --border-width: 2px;
            --border: var(--border-width) var(--border-color) solid;
            --border-radius: 8px;

            --color-primary: rgb(100, 148, 237);
            --color-primary-rgb: 100, 148, 237;
            --color-error: rgb(255, 0, 75);
        }

        html.connected {
            --border-color: white;
        }

        html.error {
            --border-color: var(--color-error);
        }

        body {
            width: 80%;
            height: 80%;
            margin: 4px;
            display: flex;
            flex-direction: column;
            border-radius: 8px;
            border: var(--border);
            overflow: hidden;
        }

        header {
            display: flex;
            flex-direction: row;
            align-items: center;
            justify-content: space-between;
            padding: 16px;
            border-bottom: var(--border);
        }

        h1 {
            margin: 0px;
        }

        button {
            color: white;
            font-size: 24px;
            border: var(--border);
            border-radius: var(--border-radius);
            margin: 0px;
            padding: 4px;
            background-color: transparent;
            transition: background-color 200ms;
            cursor: pointer;
        }

        button:hover {
            background-color: rgba(255, 255, 255, 0.2);
        }

        html:not(.connected) .switch {
            display: none;
        }

        .switch {
            position: relative;
            width: 64px;
            height: 32px;
            margin: 4px;
            cursor: pointer;
        }

        label input {
            display: none;
        }

        label span {
            display: flex;
            justify-content: center;
            align-items: center;
            transition: border-color 200ms, background-color 200ms;
            --col: var(--border-color);
        }

        .switcher {
            box-sizing: border-box;
            position: absolute;
            width: 100%;
            height: 100%;
            border: var(--border-width) var(--col) solid;
            border-radius: 16px;
        }

        input:hover+span {
            background-color: rgba(255, 255, 255, 0.1);
        }

        input:checked+span {
            --col: var(--color-primary);
            background-color: rgba(var(--color-primary-rgb), 0.2);
            color: var(--col);
        }

        span.switcher::after {
            content: "";
            position: absolute;
            background-color: var(--col);
            width: 16px;
            height: 16px;
            border-radius: 50%;
            transform: translateX(-16px);
            transition: transform 200ms, background-color 200ms;
        }

        input:checked+span.switcher::after {
            transform: translateX(16px);
        }

        .body {
            flex: 1;
            display: flex;
            align-items: center;
            justify-content: center;
        }

        .panel {
            display: flex;
            flex-direction: column;
            background-color: var(--surface-color);
            border: var(--border);
            border-radius: var(--border-radius);
            margin: 16px;
        }

        .panel.pad {
            padding: 8px;
        }

        .panel .head {
            display: flex;
            flex-direction: row;
            align-items: center;
            height: 64px;
        }

        .panel .head>span {
            margin: 4px;
        }

        .color-panel {
            flex-direction: row;
        }

        .enabled-panel {
            display: none;
            transition: opacity 200ms;
        }

        html:not(.enabled) .enabled-panel {
            opacity: 0.5;
        }

        html.connected .enabled-panel {
            display: flex;
        }

        .disconnected-panel {
            display: none;
        }

        html.disconnected .disconnected-panel {
            display: flex;
            justify-content: center;
            align-items: center;
            background-color: var(--color-error);
            --border-color: white;
            --border: 2px var(--border-color) solid;
        }

        #color-circle {
            flex: 1;
            width: 256px;
            height: 256px;
            margin: 8px 16px;
            border-radius: 50%;
            display: flex;
            align-items: flex-end;
            justify-content: flex-end;
            transition: background-color 200ms;
        }

        #white-toggle {
            width: 64px;
            height: 64px;
            border: var(--surface-color) 8px solid;
            border-radius: 50%;
            background-color: white;
            transition: border-width 200ms, background-color 200ms;
        }

        #white-toggle:hover {
            border-width: 6px;
        }

        .slider {
            width: 28px;
            margin: 2px;
            padding: 4px;
            box-sizing: border-box;
            appearance: none;
            border-radius: 14px;
            border: var(--border);
            background-color: transparent;
            cursor: pointer;
        }

        .slider::-moz-range-thumb {
            width: 16px;
            height: 16px;
            border: none;
            border-radius: 50%;
            background-color: white;
            cursor: grab;
        }

        .slider:active::-moz-range-thumb {
            cursor: unset;
            box-sizing: border-box;
            border: var(--border-width) var(--surface-color) solid;
        }

        .slider:active {
            cursor: grabbing;
        }

        .slider.rainbow:active {
            background: linear-gradient(to top,
                    hsl(0, 100%, 50%),
                    hsl(20, 100%, 50%),
                    hsl(40, 100%, 50%),
                    hsl(60, 100%, 50%),
                    hsl(80, 100%, 50%),
                    hsl(100, 100%, 50%),
                    hsl(120, 100%, 50%),
                    hsl(140, 100%, 50%),
                    hsl(160, 100%, 50%),
                    hsl(180, 100%, 50%),
                    hsl(200, 100%, 50%),
                    hsl(220, 100%, 50%),
                    hsl(240, 100%, 50%),
                    hsl(260, 100%, 50%),
                    hsl(280, 100%, 50%),
                    hsl(300, 100%, 50%),
                    hsl(320, 100%, 50%),
                    hsl(340, 100%, 50%),
                    hsl(360, 100%, 50%));
        }

        div.animation-radio {
            width: 96px;
            display: flex;
            align-self: stretch;
            justify-content: space-evenly;
            flex-direction: column;
            border-right: var(--border);
        }

        label.animation-radio {
            flex: 1;
            display: flex;
        }

        label.animation-radio span.radio {
            width: 100%;
        }
        
        @media (max-width: 650px) or (orientation: portrait) {
            body {
                margin: 0px;
                width: 100%;
                height: 100%;
                border: none;
            }
            
            .color-panel {
                flex-direction: column-reverse;
            }
            
            div.animation-radio {
                height: 64px;
                width: 100%;
                flex-direction: row;
                border-right: none;
                border-top: var(--border);
            }
        }
    </style>
    <script>
        var ws;

        var colorPreview;

        var colorCircle;
        var whiteToggle;

        var statusText;
        var enabledSwitch;
        var brightnessSlider;
        var colorSlider;

        var stripStatus;

        function start() {
            statusText = document.getElementById("status");
            enabledSwitch = document.getElementById("enabled-switch")

            colorCircle = document.getElementById("color-circle");
            whiteToggle = document.getElementById("white-toggle");
            colorPreview = colorCircle;

            brightnessSlider = document.getElementById("brightness-slider");
            colorSlider = document.getElementById("color-slider");

            connect();
        }

        function connect() {
            ws?.close();
            statusText.innerText = "Connecting...";
            ws = new WebSocket("ws://" + window.location.hostname + ":81");
            // ws = new WebSocket("ws://192.168.1.123:81");
            ws.onopen = function (evt) {
                console.log('websocket open');
                document.documentElement.classList.add("connected");
                document.documentElement.classList.remove("disconnected");
                document.documentElement.classList.remove("error");
            };
            ws.onclose = function (evt) {
                console.log('websocket close');
                statusText.innerText = "Connection lost";
                document.documentElement.classList.remove("connected");
                document.documentElement.classList.add("disconnected");
                document.documentElement.classList.remove("error");
            };
            ws.onerror = function (evt) {
                console.log(evt);
                document.documentElement.classList.add("error");
            };
            ws.onmessage = onMessage;
        }

        function send(message) {
            let json = JSON.stringify(message);
            console.log(json);
            ws.send(json);
        }

        function onMessage(event) {
            console.log(event.data);
            let data = JSON.parse(event.data);
            stripStatus = data;


            setEnabled(stripStatus["enabled"]);
            setIsWhite(stripStatus["isWhite"]);
            setColorHue(stripStatus["hue"]);
            setBrightness(stripStatus["brightness"]);
            setAnimation(stripStatus["anim"]);

        }

        function setEnabled(enabled) {
            statusText.innerText = enabled ? "On" : "Off";
            enabledSwitch.checked = enabled;

            if (enabled)
                document.documentElement.classList.add("enabled");
            else
                document.documentElement.classList.remove("enabled");
        }

        function sendEnabled(enabled) {
            send({
                "enabled": enabled
            });
        }

        function setColorHue(hue) {
            let col = "hsl(" + hue + ", 100%, 50%)";
            //document.body.style.boxShadow = "0px 0px 64px " + col;
            if (colorPreview != null)
                colorPreview.style.backgroundColor = col;
            colorSlider.value = hue;
        }

        function sendColorHue(hue) {
            send({
                "hue": hue
            });
        }

        function setBrightness(brightness) {
            brightnessSlider.value = brightness;
        }

        function sendBrightness(brightness) {
            send({
                "brightness": brightness
            });
        }

        function setIsWhite(isWhite) {
            let other;
            if (isWhite) {
                colorPreview = whiteToggle;
                other = colorCircle;
            } else {
                colorPreview = colorCircle;
                other = whiteToggle;
            }

            other.style.backgroundColor = "white";
        }

        function sendIsWhite(white) {
            if (white == null)
                white = !stripStatus["isWhite"];
            send({
                "isWhite": white
            });
        }


        function setAnimation(animation) {
            let id = { "0": "none", "1": "rainbow", "2": "fade" }[animation];
            id = "radio-anim-" + id;

            let element = document.getElementById(id);
            element.checked = true;
        }

        function sendAnimation(animation) {
            send({
                "anim": animation
            });
        }

    </script>
</head>

<body onload="start()">
    <header>
        <h1 id="status">Connecting...</h1>
        <label class="switch">
            <input type="checkbox" id="enabled-switch" onchange="sendEnabled(this.checked)">
            <span class="switcher"></span>
        </label>
    </header>

    <div class="body">

        <div class="panel color-panel enabled-panel">
            <div class="animation-radio">
                <label class="animation-radio">
                    <input type="radio" name="animation" id="radio-anim-none" value="0"
                        onchange="sendAnimation(this.value)">
                    <span class="radio">None</span>
                </label>
                <label class="animation-radio">
                    <input type="radio" name="animation" id="radio-anim-rainbow" value="1"
                        onchange="sendAnimation(this.value)">
                    <span class="radio">Rainbow</span>
                </label>
                <label class="animation-radio">
                    <input type="radio" name="animation" id="radio-anim-fade" value="2"
                        onchange="sendAnimation(this.value)">
                    <span class="radio">Fade</span>
                </label>
            </div>
            <div style="display: flex; padding: 8px;">
                <input type="range" orient="vertical" id="color-slider" class="slider rainbow" min="0" max="360" value="0"
                    onchange="sendColorHue(this.value)" oninput="setColorHue(this.value)">
                <div id="color-circle">
                    <button id="white-toggle" onclick="sendIsWhite()"></button>
                </div>
                <input type="range" orient="vertical" id="brightness-slider" class="slider" min="1" max="255" value="255"
                    onchange="sendBrightness(this.value)">
            </div>
        </div>
        <div class="panel disconnected-panel pad">
            <h2>Connection lost</h2>
            <button onclick="connect()">Reconnect</button>
        </div>
    </div>
</body>

</html>

)--";
