module main

go 1.22

require github.com/go-sql-driver/mysql v1.8.1

require filippo.io/edwards25519 v1.1.0 // indirect

replace github.com/go-sql-driver/mysql => ./pkg/github.com/go-sql-driver/mysql@v1.8.1

replace filippo.io/edwards25519 => ./pkg/filippo.io/edwards25519@v1.1.0
