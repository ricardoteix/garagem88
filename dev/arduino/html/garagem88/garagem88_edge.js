/*jslint */
/*global AdobeEdge: false, window: false, document: false, console:false, alert: false */
(function (compId) {

    "use strict";
    var im='images/',
        aud='media/',
        vid='media/',
        js='js/',
        fonts = {
        },
        opts = {
            'gAudioPreloadPreference': 'auto',
            'gVideoPreloadPreference': 'auto'
        },
        resources = [
        ],
        scripts = [
        ],
        symbols = {
            "stage": {
                version: "5.0.1",
                minimumCompatibleVersion: "5.0.0",
                build: "5.0.1.386",
                scaleToFit: "none",
                centerStage: "none",
                resizeInstances: false,
                content: {
                    dom: [
                        {
                            id: 'bg',
                            type: 'image',
                            rect: ['0px', '0px', '640px', '1136px', 'auto', 'auto'],
                            fill: ["rgba(0,0,0,0)",im+"bg.png",'0px','0px']
                        },
                        {
                            id: 'alarmeOn',
                            type: 'image',
                            rect: ['152px', '230px', '128px', '128px', 'auto', 'auto'],
                            cursor: 'pointer',
                            fill: ["rgba(0,0,0,0)",im+"alarmeOn.png",'0px','0px']
                        },
                        {
                            id: 'alarmeOff',
                            type: 'image',
                            rect: ['350px', '230px', '128px', '128px', 'auto', 'auto'],
                            cursor: 'pointer',
                            fill: ["rgba(0,0,0,0)",im+"alarmeOff.png",'0px','0px']
                        },
                        {
                            id: 'portao',
                            type: 'image',
                            rect: ['256px', '568px', '128px', '128px', 'auto', 'auto'],
                            cursor: 'pointer',
                            fill: ["rgba(0,0,0,0)",im+"portao.png",'0px','0px']
                        },
                        {
                            id: 'txtPortao2',
                            type: 'image',
                            rect: ['236px', '466px', '168px', '54px', 'auto', 'auto'],
                            fill: ["rgba(0,0,0,0)",im+"txtPortao.png",'0px','0px']
                        },
                        {
                            id: 'txtAlarme3',
                            type: 'image',
                            rect: ['227px', '123px', '185px', '55px', 'auto', 'auto'],
                            fill: ["rgba(0,0,0,0)",im+"txtAlarme.png",'0px','0px']
                        }
                    ],
                    style: {
                        '${Stage}': {
                            isStage: true,
                            rect: ['null', 'null', '640px', '1136px', 'auto', 'auto'],
                            overflow: 'hidden',
                            fill: ["rgba(255,255,255,1)"]
                        }
                    }
                },
                timeline: {
                    duration: 0,
                    autoPlay: true,
                    data: [

                    ]
                }
            }
        };

    AdobeEdge.registerCompositionDefn(compId, symbols, fonts, scripts, resources, opts);

    if (!window.edge_authoring_mode) AdobeEdge.getComposition(compId).load("garagem88_edgeActions.js");
})("EDGE-2654418491");
