#
# Makefile to build ECM Controller Client & Server
#

help:
	@echo "Build ECM Controller client & server"
	@echo "Supported Targets : "
	@echo "    help  : This message"
	@echo "    clean : Rm created files"
	@echo "    client: Build client binaries"
	@echo "    server: Build server binaries"
	@echo "    bats  : Install Bats files"
	@echo "    tests : Run unit tests"

client/ecmctl:
	(cd client && make)

server/miracle_controller:
	(cd server && make)

bats:
	mkdir -p bats
	cd bats && git submodule update

tests: | bats
	(cd bats/ && test/libs/bats/bin/bats ../client/tests/ecm_client.bats) || /bin/true
	(cd bats/ && test/libs/bats/bin/bats ../server/tests/ecm_server.bats) || /bin/true
	(cd bats/ && test/libs/bats/bin/bats ../common/tests/ecm_common.bats) || /bin/true

clean:
	rm -rf bats