# irclog2json

TiarraまたはMadokaで取得したIRCログファイルをJSONファイルに変換します。変換後のファイルはlog-archiverのデータベースへのインポートに使用することができます。

## 必要なライブラリ

文字コードの変換に[ICU](http://site.icu-project.org/)を使用しています。Debian系ではlibicu-devを、Red Hat系ではlibicu-develをインストールしてください。

## ビルド

初めにサブモジュールをダウンロードしてください。

ビルドにはC++17に対応したコンパイラ（GCCの場合7以降）とCMake 3.8以降が必要です。ビルド用のディレクトリを作成し、その中で作業してください。

```bash
# サブモジュールのダウンロード
git submodule update --init

# ビルド
mkdir build && cd build
cmake ..
make

# 並列でコンパイルする場合
# N = CPUコア数 + 1
make -j N
```

### ICUを手動でコンパイルした場合

ICUの探索にpkg-configを使用しているため、環境変数 `PKG_CONFIG_PATH` を指定してCMakeを実行してください。

```bash
PKG_CONFIG_PATH=/path/to/icu4c/lib/pkgconfig cmake ..
```

## 使い方

```
irclog2json (--tiarra | --madoka) [--iso-2022-jp] [--pretty] /path/to/yyyymmdd.txt チャンネル名
```

* ログの形式を指定する `--tiarra` または `--madoka` は必須です。
* IRCログのファイル名から日付を決定します。
* チャンネル名は先頭の "#" を除いたものです。

変換後、ログファイルと同じディレクトリに yyyymmdd.json を出力します。

### Tiarra形式からJSONに変換する場合

```
irclog2json --tiarra /path/to/yyyymmdd.txt チャンネル名
```

### Madoka形式からJSONに変換する場合

```
irclog2json --madoka /path/to/yyyymmdd.txt チャンネル名
```

### 文字コードがISO-2022-JPの場合

上記に加えてオプション `--iso-2022-jp` を付けてください。

```
irclog2json --madoka --iso-2022-jp /path/to/yyyymmdd.txt チャンネル名
```

### 読みやすい書式で出力する

オプション `--pretty` を付けてください。

```
irclog2json --tiarra --pretty /path/to/yyyymmdd.txt チャンネル名
```

### データベースへのインポート

import.rbを使います。変換後、以下のコマンドでlog-archiverのデータベースにログをインポートすることができます。

```bash
cd /path/to/log-archiver
bin/rails runner -e production /path/to/import.rb /path/to/yyyymmdd.json
```
