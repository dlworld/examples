
Name:             glance_store
Epoch:            1
Version:          0.9.1
Release:          3
Summary:          glance_store

License:          ASL 2.0
URL:              http://www.openstack.org/software/openstack-storage/
Source0:          %{name}-%{version}.tar.gz

BuildArch:        noarch

%description
glance_store

%prep
%setup -q
%autosetup -n %{name}-%{version} -S git

%build
%{__python2} setup.py build

%install
%{__python2} setup.py install -O1 --skip-build --root %{buildroot}
python_path=%{buildroot}/%{python2_sitelib}/%{name}
#python -c "import compileall; compileall.compile_dir('${python_path}')"
#find  ${python_path}  -name  '*.py'  -type  f  -exec  rm  -rf  {} \;


%pre

%post

%preun

%postun

%files
/usr/bin/glance-rootwrap
%{python2_sitelib}/glance_store
%{python2_sitelib}/glance_store-*.egg-info

%changelog
