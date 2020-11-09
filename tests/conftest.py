import pytest, subprocess, shutil, os, sys
import constant

def pytest_addoption(parser):
    parser.addoption(
        "--github", action="store_true", default=False, help="for running tests on github"
    )

def pytest_collection_modifyitems(config, items):
	if config.getoption("--github"):
		constant.github = 'true'
