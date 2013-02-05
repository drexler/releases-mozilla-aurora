# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

webidl_base = $(topsrcdir)/dom/webidl

generated_webidl_files = \
  CSS2Properties.webidl \
  $(NULL)

webidl_files = \
  AudioBuffer.webidl \
  AudioBufferSourceNode.webidl \
  AudioContext.webidl \
  AudioDestinationNode.webidl \
  AudioListener.webidl \
  AudioNode.webidl \
  AudioParam.webidl \
  AudioSourceNode.webidl \
  BatteryManager.webidl \
  BiquadFilterNode.webidl \
  Blob.webidl \
  CanvasRenderingContext2D.webidl \
  CaretPosition.webidl \
  CDATASection.webidl \
  CFStateChangeEvent.webidl \
  CharacterData.webidl \
  ClientRectList.webidl \
  Comment.webidl \
  CSS.webidl \
  CSSPrimitiveValue.webidl \
  CSSStyleDeclaration.webidl \
  CSSValue.webidl \
  CSSValueList.webidl \
  DelayNode.webidl \
  Document.webidl \
  DocumentFragment.webidl \
  DocumentType.webidl \
  DOMImplementation.webidl \
  DOMParser.webidl \
  DOMSettableTokenList.webidl \
  DOMStringMap.webidl \
  DOMTokenList.webidl \
  DOMTransaction.webidl \
  DummyBinding.webidl \
  DynamicsCompressorNode.webidl \
  Element.webidl \
  EventHandler.webidl \
  EventListener.webidl \
  EventSource.webidl \
  EventTarget.webidl \
  File.webidl \
  FileHandle.webidl \
  FileList.webidl \
  FileReaderSync.webidl \
  FormData.webidl \
  Function.webidl \
  GainNode.webidl \
  HTMLAnchorElement.webidl \
  HTMLBodyElement.webidl \
  HTMLBRElement.webidl \
  HTMLCollection.webidl \
  HTMLDataListElement.webidl \
  HTMLDivElement.webidl \
  HTMLDListElement.webidl \
  HTMLDocument.webidl \
  HTMLElement.webidl \
  HTMLFontElement.webidl \
  HTMLFrameSetElement.webidl \
  HTMLHeadingElement.webidl \
  HTMLImageElement.webidl \
  HTMLLabelElement.webidl \
  HTMLLIElement.webidl \
  HTMLOListElement.webidl \
  HTMLOptionsCollection.webidl \
  HTMLParagraphElement.webidl \
  HTMLPreElement.webidl \
  HTMLPropertiesCollection.webidl \
  HTMLScriptElement.webidl \
  HTMLSpanElement.webidl \
  HTMLStyleElement.webidl \
  HTMLTableCaptionElement.webidl \
  HTMLTableCellElement.webidl \
  HTMLTableColElement.webidl \
  HTMLTableElement.webidl \
  HTMLTableRowElement.webidl \
  HTMLTableSectionElement.webidl \
  HTMLTitleElement.webidl \
  HTMLUListElement.webidl \
  ImageData.webidl \
  LinkStyle.webidl \
  Location.webidl \
  MutationObserver.webidl \
  Node.webidl \
  NodeFilter.webidl \
  NodeList.webidl \
  PaintRequest.webidl \
  PaintRequestList.webidl \
  PannerNode.webidl \
  Performance.webidl \
  PerformanceNavigation.webidl \
  PerformanceTiming.webidl \
  ProcessingInstruction.webidl \
  Rect.webidl \
  RGBColor.webidl \
  RTCIceServer.webidl \
  Screen.webidl \
  SVGAElement.webidl \
  SVGAltGlyphElement.webidl \
  SVGAngle.webidl \
  SVGAnimatedAngle.webidl \
  SVGAnimatedBoolean.webidl \
  SVGAnimatedLength.webidl \
  SVGAnimatedLengthList.webidl \
  SVGAnimatedNumberList.webidl \
  SVGAnimatedPathData.webidl \
  SVGAnimatedPoints.webidl \
  SVGAnimatedPreserveAspectRatio.webidl \
  SVGAnimatedTransformList.webidl \
  SVGAnimateElement.webidl \
  SVGAnimateMotionElement.webidl \
  SVGAnimateTransformElement.webidl \
  SVGAnimationElement.webidl \
  SVGCircleElement.webidl \
  SVGClipPathElement.webidl \
  SVGDefsElement.webidl \
  SVGDescElement.webidl \
  SVGElement.webidl \
  SVGEllipseElement.webidl \
  SVGFitToViewBox.webidl \
  SVGForeignObjectElement.webidl \
  SVGGElement.webidl \
  SVGGradientElement.webidl \
  SVGGraphicsElement.webidl \
  SVGImageElement.webidl \
  SVGLengthList.webidl \
  SVGLinearGradientElement.webidl \
  SVGLineElement.webidl \
  SVGMarkerElement.webidl \
  SVGMaskElement.webidl \
  SVGMatrix.webidl \
  SVGMetadataElement.webidl \
  SVGMPathElement.webidl \
  SVGNumberList.webidl \
  SVGPathElement.webidl \
  SVGPathSeg.webidl \
  SVGPathSegList.webidl \
  SVGPatternElement.webidl \
  SVGPoint.webidl \
  SVGPointList.webidl \
  SVGPolygonElement.webidl \
  SVGPolylineElement.webidl \
  SVGPreserveAspectRatio.webidl \
  SVGRadialGradientElement.webidl \
  SVGRectElement.webidl \
  SVGScriptElement.webidl \
  SVGSetElement.webidl \
  SVGStopElement.webidl \
  SVGStyleElement.webidl \
  SVGSVGElement.webidl \
  SVGSwitchElement.webidl \
  SVGSymbolElement.webidl \
  SVGTests.webidl \
  SVGTextContentElement.webidl \
  SVGTextElement.webidl \
  SVGTextPathElement.webidl \
  SVGTextPositioningElement.webidl \
  SVGTitleElement.webidl \
  SVGTransform.webidl \
  SVGTransformList.webidl \
  SVGTSpanElement.webidl \
  SVGUnitTypes.webidl \
  SVGUseElement.webidl \
  SVGURIReference.webidl \
  SVGViewElement.webidl \
  SVGZoomAndPan.webidl \
  Text.webidl \
  TextDecoder.webidl \
  TextEncoder.webidl \
  URL.webidl \
  ValidityState.webidl \
  WebSocket.webidl \
  UndoManager.webidl \
  USSDReceivedEvent.webidl \
  XMLHttpRequest.webidl \
  XMLHttpRequestEventTarget.webidl \
  XMLHttpRequestUpload.webidl \
  XMLSerializer.webidl \
  XMLStylesheetProcessingInstruction.webidl \
  XPathEvaluator.webidl \
  XULElement.webidl \
  $(NULL)

ifdef MOZ_WEBGL
webidl_files += \
  WebGLRenderingContext.webidl \
  $(NULL)
endif

ifdef MOZ_WEBRTC
webidl_files += \
  MediaStreamList.webidl \
  $(NULL)
endif

ifdef ENABLE_TESTS
test_webidl_files := \
  TestCodeGen.webidl \
  TestDictionary.webidl \
  TestExampleGen.webidl \
  TestTypedef.webidl \
  $(NULL)
else
test_webidl_files := $(NULL)
endif

