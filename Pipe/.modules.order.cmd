cmd_/media/sf_SistemasOperativos/Pipe/modules.order := {   echo /media/sf_SistemasOperativos/Pipe/pipe.ko; :; } | awk '!x[$$0]++' - > /media/sf_SistemasOperativos/Pipe/modules.order
