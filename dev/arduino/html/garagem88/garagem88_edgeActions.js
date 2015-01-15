/***********************
* Adobe Edge Animate Composition Actions
*
* Edit this file with caution, being careful to preserve 
* function signatures and comments starting with 'Edge' to maintain the 
* ability to interact with these actions from within Adobe Edge Animate
*
***********************/
(function($, Edge, compId){
var Composition = Edge.Composition, Symbol = Edge.Symbol; // aliases for commonly used Edge classes

   //Edge symbol: 'stage'
   (function(symbolName) {
      
      
      Symbol.bindTriggerAction(compId, symbolName, "Default Timeline", 0, function(sym, e) {
         console.log("toma");

      });
      //Edge binding end

      Symbol.bindElementAction(compId, symbolName, "${alarmeOn}", "click", function(sym, e) {
         $.ajax({
             type: 'POST',
             url: 'garagem88.html',
             data: { 
                 'buzz': '11'
             },
             success: function(msg){
                 console.log(msg);
             }
         });

      });
      //Edge binding end

      

      Symbol.bindElementAction(compId, symbolName, "${portao}", "click", function(sym, e) {
         $.ajax({
             type: 'POST',
             url: 'garagem88.html',
             data: { 
                  'buzz': '10'
             },
             success: function(msg){
                 console.log(msg);
             }
         });

      });
      //Edge binding end

      Symbol.bindElementAction(compId, symbolName, "${alarmeOff}", "click", function(sym, e) {
         $.ajax({
             type: 'POST',
             url: 'garagem88.html',
             data: { 
                  'buzz': '12'
             },
             success: function(msg){
                 console.log(msg);
             }
         });
         

      });
      //Edge binding end

   })("stage");
   //Edge symbol end:'stage'

})(window.jQuery || AdobeEdge.$, AdobeEdge, "EDGE-2654418491");