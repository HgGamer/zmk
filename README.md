# ZMK Firmware: Fork of urob's fork

This is my fork of urob's personal ZMK fork containing various experimental features. ~~It is regularly rebased
onto the latest upstream.~~

I made this fork while we wait for urob to update his.

This is rebased with zmk/main as of 2024-01-06.

> **Warning**
>
> This fork uses the experimental and potentially unstable zephyr 3.5 version.
>
> If you don't want to take the risk of running this experimental version and don't need mouse keys, you can use the [urob/no-mouse branch](https://github.com/infused-kim/zmk/tree/urob/no-mouse).

## Included features and PRs...

Below is a list of features currently included in the `main` branch _on top of_
the official ZMK master branch.

- **Mouse Keys using Zephyr 3.5** ([PR #2027](https://github.com/zmkfirmware/zmk/pull/2027) + [PR #1995](https://github.com/zmkfirmware/zmk/pull/1995))
- **swapper** (PR [#1366](https://github.com/zmkfirmware/zmk/pull/1366)) - official PR + fixes needed for Zephyr 3.2 and 3.5
- **smart-word** (PR [#1451](https://github.com/zmkfirmware/zmk/pull/1451)) - official PR, updated to Zephyr-3.2
- **fix-key-repeat** - fix [key-repeat rolling issue](https://github.com/zmkfirmware/zmk/issues/1207)
- **on-release-for-tap-preferred** - [on-release option for tap-preferred](https://github.com/celejewski/zmk/commit/d7a8482712d87963e59b74238667346221199293) by Andrzej
- **adv360pro** (PR [#1454](https://github.com/zmkfirmware/zmk/pull/1454)) - offical PR
- **zen-tweaks** - [display & battery improvements](https://github.com/caksoylar/zmk/tree/caksoylar/zen-v1%2Bv2) by Cem Aksoylar

Thanks to caksoylar for figuring out the changes needed for the swapper PR. As well as to him and petejohanson for their work on the zephyr 3.5 mouse PR.

And of course to urob for his amazing fork.

## How to use it

### Adjust your mouse key config

The mouse key config changed slightly compared to the old version.

The scroll behavior has been renamed from `&mwh` to `msc`.

And the parameters from `SCROLL_XXX` to `SCRL_XXX`.

You can see a [working example here](https://github.com/infused-kim/zmk-config/blob/main/config/includes/mouse_keys.dtsi).


### Github Actions

In order to use this branch with Github Actions, replace the contents of `west.yml` in
your `zmk-config/config` directory with the following contents:

```yaml
manifest:
  remotes:
    - name: zmkfirmware
      url-base: https://github.com/zmkfirmware
    - name: urob
      url-base: https://github.com/urob
    - name: infused-kim
      url-base: https://github.com/infused-kim
  projects:
    - name: zmk
      remote: infused-kim
      revision: urob/mouse
      import: app/west.yml
  self:
    path: config
```

And `.github/workflows/build.yml` with...

```yaml
on: [push, pull_request, workflow_dispatch]

jobs:
  build:
    uses: petejohanson/zmk/.github/workflows/build-user-config.yml@core/zephyr-3.5-update
```

### VSCode Build

- Follow the [official toolchain setup instructions](https://zmk.dev/docs/development/setup)
- Switch the zmk code to this branch
- Switch the zephyr code to the [petejohanson/zmk-v2.3.0-with-fixes](https://github.com/petejohanson/zephyr/tree/v3.5.0%2Bzmk-fixes) branch
- In VSCode run the `Remote Containers: Rebuild Container Without Cache` action [as described here](https://zmk.dev/blog/2023/04/06/zephyr-3-2)
