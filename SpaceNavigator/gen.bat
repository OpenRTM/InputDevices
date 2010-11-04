@echo off

C:\"Program Files"\OpenRTM-aist\utils\rtc-template\rtc-template.py -bcxx -bvcproject^
 --module-name=SpaceNavigator --module-desc="3DConnexion SpaceNavigator component"^
 --module-version=0.1 --module-vendor=AIST --module-category=input_device^
 --module-comp-type=DataFlowComponent --module-act-type=SPORADIC^
 --module-max-inst=1^
 --config=sensitivity:double:1.0 --config=translation_scale:double:1.0^
 --config=rotation_scale:double:1.0^
 --outport=pos:TimedDoubleSeq --outport=button:TimedShortSeq

pause