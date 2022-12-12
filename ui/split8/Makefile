#
# Makefile to build ECM Controller Client & Server
#

help:
	@echo "Build ECM Controller client & server"
	@echo "Supported Targets : "
	@echo "    help  : This message"
	@echo "    clean : Rm created files"
	@echo "    bats  : Install Bats files"
	@echo "    tests : Run unit tests"

bats:
	mkdir -p bats
	cd bats && git submodule update

tests: bats
	(cd bats/ && test/libs/bats/bin/bats ../client/tests/ecm_client.bats)
	(cd bats/ && test/libs/bats/bin/bats ../server/tests/ecm_server.bats)

clean:
	rm -rf bats