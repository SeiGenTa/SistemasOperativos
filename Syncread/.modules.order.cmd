cmd_/media/sf_SistemasOperativos/Syncread/modules.order := {   echo /media/sf_SistemasOperativos/Syncread/syncread.ko; :; } | awk '!x[$$0]++' - > /media/sf_SistemasOperativos/Syncread/modules.order
