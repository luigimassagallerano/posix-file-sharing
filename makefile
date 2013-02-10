SUBDIRS = socket utility strutture src_DFR src_FS test/notificheMultiple test/notificheMultiple/FS test/notificheMultiple/Client/client1 test/notificheMultiple/Client/client2 test/notificheMultiple/Client/client3 test/notificheMultiple/Client/client4 test/notificheMultiple/Client/client5 test/notificheMultiple/Client/client6 test/notificheMultiple/Client/client7 test/notificheMultiple/Client/client8 test/notificheMultiple/Client/client9 test/notificheMultiple/Client/client10 test/accessoConcorrente/Client/client10 test/accessoConcorrente/Client/client9 test/accessoConcorrente/Client/client8 test/accessoConcorrente/Client/client7 test/accessoConcorrente/Client/client6 test/accessoConcorrente/Client/client5 test/accessoConcorrente/Client/client4 test/accessoConcorrente/Client/client3 test/accessoConcorrente/Client/client2 test/accessoConcorrente/Client/client1 test/accessoConcorrente test/accessoConcorrente/FS

all: subs

subs:
	for dir in $(SUBDIRS) ; do make -C $$dir ; done

clean:
	for dir in $(SUBDIRS) ; do make clean -C $$dir ; done
