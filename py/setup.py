from setuptools import setup, Extension
import os

def main():
	sources = [ "src/"+ file for file in os.listdir('./src') if file.endswith('.cpp') ]
	setup(
		name='autoplan',
		version='0.0.1',
		ext_modules=[
			Extension(
					'autoplan',
					sources=sources,
					include_dirs=["../src"],
					library_dirs=["../bin/Debug"],
					libraries=["autoplan"],
					extra_compile_args=["-O0", "-ggdb"],
					extra_link_args=["-ggdb"]
			)
		]
	)

if __name__ == "__main__":
	main()