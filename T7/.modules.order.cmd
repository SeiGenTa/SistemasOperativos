cmd_/media/sf_SistemasOperativos/T7/modules.order := {   echo /media/sf_SistemasOperativos/T7/prodcons.ko; :; } | awk '!x[$$0]++' - > /media/sf_SistemasOperativos/T7/modules.order
