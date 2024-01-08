// ==UserScript==
// @name         trambiya
// @namespace    http://tampermonkey.net/
// @version      0.1
// @description  try to take over the world!
// @author       You
// @match        https://bus-med.1337.ma/
// @icon         https://www.google.com/s2/favicons?sz=64&domain=tampermonkey.net
// @grant        none
// ==/UserScript==

function SetAllTransitionsToZero() {
    const elems = document.querySelectorAll("*")
    elems.forEach((elem) => {
        elem.style.transition = "none"
    })
}

(function () {
    'use strict';
    const elems = document.querySelectorAll(".card")
    const modalcontent = document.querySelectorAll(".modal-content")
    const d = new Date();

    const breakpoint = 59.975;
    const hour_switch = 50;

    let minutes = d.getMinutes() + (d.getSeconds() / 60);
    let hours = d.getHours();
    let bushour = (minutes > hour_switch ? (hours + 2) : (hours + 1)) % 24
    SetAllTransitionsToZero();
    console.log("looking for " + bushour)
    console.log(minutes)
    modalcontent.forEach((elem) => { elem.setAttribute("style", "margin-top: 300px;") })
    elems.forEach((elem) => {
        if (!(elem.innerText.includes("Martil") || elem.innerText.includes("karama")))
        {
            elem.remove()
        }
    })
    const click = Array.from(document.querySelectorAll(".card.available  .claimbutton:nth-child(2)")).find((elem) => {
        return elem.parentElement.parentElement.textContent.includes(`${bushour}:`)
    })
    if (minutes < breakpoint && minutes > 30) {
        console.log("about to reload in " + (breakpoint - minutes) + " minutes")
        setTimeout(() => {
            location.reload()
        }, (breakpoint - minutes) * 60 * 1000)
    }
    else {
        if (minutes > 50 || !click) {
            if (!document.querySelector(".card.reserved"))
            {
                location.reload()
            }
        }
        else if (click) {
            click.click();
            while (1)
            {
                const captcha = document.querySelector(".captcha")
                if (captcha)
                {
                    const image = new Image()
                    image.setAttribute("style", "position: absolute; top: 0; left: 0; z-index: 99999; width: 300px; filter: invert(0) contrast(1)")
                    image.src = captcha.src
                    document.body.appendChild(image)
                    // iamge on load
                    image.onload = () => {
                        while (1)
                        {
                            const box = document.querySelector(`input[name="captcha_1"]`)
                            if (box)
                            {
                                setInterval(() => {
                                    box.focus()
                                }, 50)
                                break;
                            }
                        }
                    }
                    break;
                }
            }
        }
        else
        {
            console.log("my brother in christ")
        }
    }
})();
