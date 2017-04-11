# irclog2json

Tiarraで取得したIRCログファイルをJSONファイルに変換します。変換後のファイルはlog-archiverのデータベースへのインポートに使用することができます。

## ビルド

初めにサブモジュールをダウンロードしてください。

ビルドにはCMakeが必要です。ビルド用のディレクトリを作成し、その中で作業してください。

```bash
# サブモジュールのダウンロード
git submodule update --init

# ビルド
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make

# 並列でコンパイルする場合
# N = CPUコア数 + 1
make -j N
```

## 使い方

```
irclog2json /path/to/yyyymmdd.txt チャンネル名
```

* IRCログのファイル名から日付を決定します。
* チャンネル名は先頭の "#" を除いたものです。

変換後、ログファイルと同じディレクトリに yyyymmdd.json を出力します。

### データベースへのインポート

import.rbを使います。変換後、以下のコマンドでlog-archiverのデータベースにログをインポートすることができます。

```bash
cd /path/to/log-archiver
bin/rails runner -e production /path/to/import.rb /path/to/yyyymmdd.json
```
