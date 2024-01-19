#!/usr/bin/env python3
import os
import html


def print_html():
    html_content = """
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>upload File</title>

</head>

<body>
    <style>
        @import url("https://fonts.googleapis.com/css?family=Source+Code+Pro:400,500,600,700|Source+Sans+Pro:400,600,700&display=swap");
        body {
            background: #cf87f0;
            font-family: "Source Sans Pro", sans-serif;
            font-size: 16px;
        }
        
        * {
            box-sizing: border-box;
        }
        
        .wrapper {
            min-height: 100vh;
            display: flex;
            padding: 50px 15px;
            justify-content: center;
            align-items: center;
        }
        
        @media screen and (max-width: 700px),
        (max-height: 500px) {
            .wrapper {
                flex-wrap: wrap;
                flex-direction: column;
            }
        }
        
        .card-form {
            max-width: 570px;
            margin: auto;
            width: 100%;
        }
        
        @media screen and (max-width: 576px) {
            .card-form {
                margin: 0 auto;
            }
        }
        
        .card-form__inner {
            background: #fff;
            box-shadow: 0 30px 60px 0 rgba(90, 116, 148, 0.4);
            border-radius: 10px;
            padding: 35px;
            padding-top: 80px;
        }
        
        h1 {
            text-align: center;
        }
        
        @media screen and (max-width: 480px) {
            .card-form__inner {
                padding: 25px;
                padding-top: 165px;
            }
        }
        
        @media screen and (max-width: 360px) {
            .card-form__inner {
                padding: 15px;
                padding-top: 165px;
            }
        }
    </style>
    <div class="wrapper" id="app">
        <div class="card-form">
            <div class="card-form__inner">
                <h1>cgi works</h1>
            </div>
        </div>
    </div>

</body>

</html>
    """
    print(html_content)

print_html()