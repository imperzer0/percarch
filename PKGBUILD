#!/bin/bash
# Maintained by imper <imperator999mcpe@gmail.com>

pkgname=percarh
pkgver=1.0
pkgrel=1
pkgdesc='AI based compressor'
author="imperzer0"
arch=('any')
url=https://github.com/$author/$pkgname
license=('GPL3')
depends=('gcc')
makedepends=('cmake' 'git' 'gcc' 'make')

_srcprefix="local:/"
_libfiles=("CMakeLists.txt" "main.cpp" "constants.hpp"
	"percarch.cpp" "percarch.h"
)

# shellcheck disable=SC2068
for _libfile in ${_libfiles[@]}; do
	source=(${source[@]} "$_srcprefix/$_libfile")
	sha256sums=(${sha256sums[@]} "SKIP")
done

external=(${external[@]} "https://raw.githubusercontent.com/imperzer0/perceptron/8ee566f52cc90b261e4910acb69563a842642872/layer.cpp")
sha256sums=(${sha256sums[@]} "cb3be1ccd1f57776b28efd8ad2f86f09e98900cfd28f8d7d1dee738d5a4284ea")

external=(${external[@]} "https://raw.githubusercontent.com/imperzer0/perceptron/8ee566f52cc90b261e4910acb69563a842642872/layer.hpp")
sha256sums=(${sha256sums[@]} "d1c06305cb07105411b80c80cff001d035b280a6cbe399b79195fd79d18419e0")

external=(${external[@]} "https://raw.githubusercontent.com/imperzer0/perceptron/8ee566f52cc90b261e4910acb69563a842642872/matrix.cpp")
sha256sums=(${sha256sums[@]} "6c141e4648e0b2658bbd75bd69105833f81649453f8b61b210e93b47866e036a")

external=(${external[@]} "https://raw.githubusercontent.com/imperzer0/perceptron/8ee566f52cc90b261e4910acb69563a842642872/matrix.hpp")
sha256sums=(${sha256sums[@]} "057b5609e5adc2dd014b39a088d31e25207988113b554c7b3b9c36aaa6aa2ac8")

external=(${external[@]} "https://raw.githubusercontent.com/imperzer0/perceptron/8ee566f52cc90b261e4910acb69563a842642872/perceptron.cpp")
sha256sums=(${sha256sums[@]} "8f9f853c744ac59c0783f908485da8f5f8edda50cb15536185835634b991912f")

external=(${external[@]} "https://raw.githubusercontent.com/imperzer0/perceptron/8ee566f52cc90b261e4910acb69563a842642872/perceptron.hpp")
sha256sums=(${sha256sums[@]} "a609060ad581737923379f249aefab3fcfd87b7709a75f543851fc7b3a19d9af")

source=(${source[@]} ${external[@]})

notarch_prepare() {
	# shellcheck disable=SC2068
	for ex in ${external[@]}; do
		wget "$ex"
	done
}

_package_version=" ("$pkgver"-"$pkgrel")"

build() {
	cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ \
		-DPACKAGE_VERSION="$_package_version" -DAPPNAME="$pkgname" .
	make -j 6
}

package() {
	install -Dm755 $pkgname "$pkgdir/usr/bin/$pkgname"
}

notarch_package() {
	cp -f $pkgname "$pkgdir/usr/bin/$pkgname"
	chmod 755 "$pkgdir/usr/bin/$pkgname"
}
