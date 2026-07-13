# Filesystem Shell

Progetto sviluppato per il corso di **Sistemi Operativi**.

Il progetto implementa un semplice filesystem persistente basato su file mmappati (`mmap`) e una shell interattiva per la gestione di file e directory.

## Funzionalità

- Filesystem mmappato e persistente
- Gestione di file e directory
- Supporto ai comandi richiesti dalla traccia del progetto

## Comandi supportati

| Comando | Descrizione |
|----------|-------------|
| `format <file> <size>` | Crea un nuovo filesystem |
| `mount <file>` | Monta il filesystem |
| `mkdir <dir>` | Crea una directory |
| `cd <dir>` | Cambia directory (`..` e `/` supportati) |
| `touch <file>` | Crea un file vuoto |
| `append <file> <testo>` | Aggiunge testo a un file |
| `cat <file>` | Visualizza il contenuto di un file |
| `ls` | Elenca file e directory |
| `rm <file/dir>` | Elimina un file o una directory vuota |
| `close` | Chiude il filesystem |

## Compilazione

```bash
make
```

## Esecuzione

```bash
./filesystem-shell
```

## Tecnologie utilizzate

- Linguaggio C
- POSIX API
- `mmap`
- `msync`
- `munmap`
- Makefile