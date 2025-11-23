const char* index_html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>⚙️ Control Panel</title>

    <style>
        body {
            background-color: #696969;
            display: flex;
            justify-content: center;
            text-align: center;
            flex-direction: column;
        }

        .main {
            background-color: #8A8A8A;
            display: flex;
            padding: 1em;
            align-items: center;
            border-radius: 2em;
            flex-wrap: wrap;
            justify-content: center;
            gap: 1em;
            align-items: stretch;
        }

        .OnOff input[type="submit"] {
            background-color: #548bf0;
        }
        .OnOff input[type="submit"]:hover {
            background-color: #3690f8;
            cursor: pointer;
            translate: -2px -2px;
            box-shadow: #2d5db8 2px 2px 4px;
        }
        .OnOff h3 {
            color: #acfcb3;
        }

        h2 {
            color: #f09797;
            text-shadow: 2px 2px 4px #000000;
            font: bolder 1.5em Arial, sans-serif;
        }

        p {
            color: #FFFFFF;
            font: 1em Arial, sans-serif;
            text-shadow: 2px 2px 4px #000000;
        }

        h1 {
            color: #FFFFFF;
            text-shadow: 2px 2px 4px #000000;
            font: bolder 2.5em Arial, sans-serif;
        }

        .default {
            display: flex;
            flex-direction: column;
            gap: 1em;
            padding: 1em;
            background-color: #696969;
            border-radius: 1em;
        }

        input[type="submit"] {
            background-color: #5cf05c;
            border: none;
            padding: 0.5em;
            border-radius: 0.5em;
            font: bolder 1em Arial, sans-serif;
            cursor: pointer;
            margin-top: auto;
        }

        input[type="submit"]:hover {
            background-color: #30e930;
            cursor: pointer;
            translate: -2px -2px;
            box-shadow: #147014 2px 2px 4px;
        }

        input[type="color"] {
            border: none;
            border-radius: 0.3em;
            background-color: #a8a6a6;
        }

        input[type="time"] {
            border: none;
            border-radius: 0.3em;
            background-color: #a8a6a6;
        }

        label {
            color: white;
            text-shadow: #003cff 2px 2px 4px;
            font: 1.1em Arial, sans-serif;
        }
        h3 {
                    color: #ace5fc;
                    text-shadow: 2px 2px 4px #000000;
                    font: bolder 1.5em Arial, sans-serif;
                    margin-top: 0px;
                    margin-bottom: 10px;
                }
        /* specific rainbow headings: remove the generic h3 shadow and keep gradient fill */
        #rainbow {
            margin-top: 0px;
            margin-bottom: 10px;
            font-family: sans-serif;
            font-size: 2em;
            /* cancel the generic h3 text-shadow */
            text-shadow: none;
            /* gradient fill */
            background: linear-gradient(to right, #ef5350, #f48fb1,  #2196f3, #26c6da, #43a047, #eeff41, #f9a825, #ff5722);
            -webkit-background-clip: text;
            background-clip: text;
            -webkit-text-fill-color: transparent;
        }
        
    </style>
</head>

<body>
    <h1>Welcome on the control panel</h1>
    <div class="main">
        <form class="OnOff default" method="get" action="/OnOff">
            <h3>Turn on / Off</h3>
            <input type="submit" value="On / Off">
        </form>
        <form class="scheduler default" method="get" action="/Set_scheduler">
            <div>
                <label for="now">Current time</label>
                <input type="time" name="now" id="now">
            </div>
            <div>
                <label for="start">Start time</label>
                <input type="time" name="start" id="start">
            </div>
            <div>
                <label for="end">End time</label>
                <input type="time" name="end" id="end">
            </div>

            <input type="submit" value="Change schedule">
        </form>
        <form class="color_picker default" method="get" action="/Set_color"> <!-- effect 0 -->  
            <div>
                <label for="color">Choose a color</label>
                <input type="color" name="color" id="color" value="#ff0000">
            </div>
            <input type="submit" value="Change color">
        </form>
        <form class="effect default" method="get" action="/setEffect/rainbow"> <!-- effect 1 -->  
            <h3 id="rainbow">Rainbow</h3>
            <input type="submit" value="Apply effect">
            
        </form>
         <form class="effect default" method="get" action="/setEffect/snow"> <!-- effect 2 -->  
            <h3>Snow</h3>
            <input type="submit" value="Apply effect">
            
        </form> <form class="effect default" method="get" action="/setEffect/disco"> <!-- effect 3 -->  
            <h3>Disco</h3>
            <input type="submit" value="Apply effect">
            
        </form> <form class="effect default" method="get" action="/setEffect/cicle"> <!-- effect 4 -->  
            <h3>Cicle</h3>
            <input type="submit" value="Apply effect">
        </form>
        </form>
        <form class="effect default" method="get" action="/setEffect/sparkling"> <!-- effect 5 -->  
            <h3>Sparkling</h3>
            <input type="submit" value="Apply effect">
        </form>
        <form class="effect default" method="get" action="/setEffect/mixed"> <!-- effect 6§ -->  
            <h3>Mixed</h3>
            <input type="submit" value="Apply effect">
        </form>
    </div>
</body>

</html>
)rawliteral";