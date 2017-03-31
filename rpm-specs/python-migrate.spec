
Name:             python-migrate
Epoch:            1
Version:          0.9.6
Release:          10
Summary:          migrate

License:          ASL 2.0
URL:              http://www.openstack.org/software/openstack-storage/
Source0:          sqlalchemy-migrate-%{version}.tar.gz

BuildArch:        noarch

%description
vmha

%prep
%setup -n sqlalchemy-migrate-%{version}
%autosetup -n sqlalchemy-migrate-%{version} -S git

%build
%{__python2} setup.py build

%install
%{__python2} setup.py install -O1 --skip-build --root %{buildroot}


%pre

%post

%preun

%postun

%files
%{python2_sitelib}/migrate
%{python2_sitelib}/sqlalchemy_migrate-*.egg-info
/usr/bin/migrate
/usr/bin/migrate-repository

%changelog
