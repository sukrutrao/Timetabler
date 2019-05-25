# Generate docs site locally

1. Install Ruby and Bundler.
    ```bash
    sudo apt install -y libssl-dev libreadline-dev curl
    curl -fsSL https://github.com/rbenv/rbenv-installer/raw/master/bin/rbenv-installer | bash
    echo 'export PATH="$HOME/.rbenv/bin:$PATH"' >> ~/.bashrc
    echo 'eval "$(rbenv init -)"' >> ~/.bashrc
    curl -fsSL https://github.com/rbenv/rbenv-installer/raw/master/bin/rbenv-doctor | bash
    rbenv install 2.4.4
    rbenv global 2.4.4
    gem install bundler
    ```
2. In the docs folder, run the following to install dependencies.
    ```bash
    bundle install
    ```
3. Run the following command to generate the site.
    ```bash
    bundle exec jekyll serve
    ```
    Now the site will be hosted locally at `https://localhost:4000/Timetabler/`.

---
This theme is a fork of [Cayman theme from GitHub pages](https://github.com/pages-themes/cayman/).