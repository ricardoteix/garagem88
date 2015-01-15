package
{
	import flash.display.Sprite;
	import flash.display.StageScaleMode;
	import flash.events.Event;
	import flash.events.IOErrorEvent;
	import flash.events.MouseEvent;
	import flash.events.ProgressEvent;
	import flash.net.Socket;
	
	public class Main extends Sprite
	{
		private var socket:Socket;
		private var host:String;
		private var port:int;
		
		private var view:GaragemSWC;
		public function Main()
		{
			this.addEventListener(Event.ADDED_TO_STAGE, onInit);
			
			this.port = 2001;
			this.host = "127.0.0.1";
		}
		
		protected function onInit(event:Event):void
		{
			this.stage.scaleMode = StageScaleMode.EXACT_FIT;
			
			this.socket = new Socket ();
			this.socket.addEventListener(Event.CONNECT, onConnect);
			this.socket.addEventListener(Event.CLOSE, onClose);
			this.socket.addEventListener(IOErrorEvent.IO_ERROR, onError);
			this.socket.addEventListener(ProgressEvent.SOCKET_DATA, onData);
			
			this.view = new GaragemSWC ();
			this.addChild(this.view);
			
			this.view.btnOn.addEventListener(MouseEvent.CLICK, onClickOn);
			this.view.btnOff.addEventListener(MouseEvent.CLICK, onClickOff);
			this.view.btnClose.addEventListener(MouseEvent.CLICK, onClickClose);
			this.view.btnOpen.addEventListener(MouseEvent.CLICK, onClickOpen);
			this.view.btnConectar.addEventListener(MouseEvent.CLICK, onClickConectar);
		}
		
		protected function onClickConectar(event:MouseEvent):void
		{
			this.socket.connect(this.host, this.port)
		}
		
		protected function onData(e:ProgressEvent):void
		{
			trace (this.socket.readUTFBytes(this.socket.bytesAvailable));
		}
		
		protected function onConnect(event:Event):void
		{
			this.view.status.gotoAndStop(2);
			this.view.btnConectar.visible = false;
		}
		
		protected function onClose(event:Event):void
		{
			this.view.status.gotoAndStop(1);
			this.view.btnConectar.visible = true;
		}
		
		protected function onError(e:IOErrorEvent):void
		{
			this.view.status.gotoAndStop(1);
			this.view.btnConectar.visible = true;
			trace ("onError", e.text);
		}
		
		protected function onClickOn(event:MouseEvent):void
		{
			this.socket.writeUTFBytes("#on");
			this.socket.flush();
		}
		
		protected function onClickOff(event:MouseEvent):void
		{
			this.socket.writeUTFBytes("#of");
			this.socket.flush();
		}
		
		protected function onClickOpen(event:MouseEvent):void
		{
			this.socket.writeUTFBytes("#op");
			this.socket.flush();
		}
		
		protected function onClickClose(event:MouseEvent):void
		{
			this.socket.writeUTFBytes("#cl");
			this.socket.flush();
			
		}
	}
}