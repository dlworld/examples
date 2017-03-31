
Name:             python-cloudosclient
Epoch:            1
Version:          1.0.0
Release:          10
Summary:          python-cloudosclient

License:          ASL 2.0
URL:              http://www.openstack.org/software/openstack-storage/
Source0:          %{name}-%{version}.tar.gz

BuildArch:        noarch

%description
python cloudosclient

%prep
%setup -q
%autosetup -n %{name}-%{version} -S git

%build
%{__python2} setup.py build

%install
%{__python2} setup.py install -O1 --skip-build --root %{buildroot}
compile_config="/root/cloudosclient.conf"
python /root/deletepy.py %{buildroot} ${compile_config}


%pre

%post

%preun

%postun

%files
%{python2_sitelib}/cloudosclient
%{python2_sitelib}/cloudosclient-*.egg-info

%changelog
