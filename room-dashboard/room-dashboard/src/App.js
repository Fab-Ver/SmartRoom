import {CAlert, CBadge, CButton, CCard, CCardBody, CCardFooter, CCardHeader, CCol, CContainer, CFormRange, CFormSwitch,CRow} from '@coreui/react'
import '@coreui/coreui/dist/css/coreui.min.css'
import './App.css'
import { useEffect, useState } from 'react'
import React from 'react';
import axios from 'axios'
import  CIcon  from '@coreui/icons-react';
import { cilSync } from '@coreui/icons';

import {
	Chart as ChartJS,
	TimeScale,
	CategoryScale,
	LinearScale,
	PointElement,
	LineElement,
	Title,
	Tooltip,
	Legend,
  } from 'chart.js';
import 'chartjs-adapter-date-fns';
import { it } from 'date-fns/locale';
import { Line } from 'react-chartjs-2';

ChartJS.register(
	TimeScale,
	CategoryScale,
	LinearScale,
	PointElement,
	LineElement,
	Title,
	Tooltip,
	Legend,
);
  
const App = () => {
	const [manageLight, setManageLight] = useState(false);
  	const [manageRollerBlind, setManageRollerBlind] = useState(50);
  	const [currentLightState, setCurrentLightState] = useState(null);
  	const [currentRollerBlindState, setCurrentRollerBlindState] = useState(null);

  	const [visibleAlert,setVisibleAlert] = useState(false);
  	const [alertState,setAlertState] = useState(false);

  	const [lightData, setLightData] = useState({
		label : [],
		data : []
  	});

  	const [rollerBlindData, setRollerBlindData] = useState({
		label : [],
		data : []
  	});

  	useEffect(() =>{
		const interval = setInterval(() => getCurrentState(), 1000)
		updateLightChart();
		updateRollerBlindChart();
		return () => clearInterval(interval)
  	}, []);

  	const getCurrentState = () => {
		axios.post('/current_state', {}, {
			headers: {
		  		'Content-Type': 'multipart/form-data',
		  		'Accept': 'application/json',
			},
	  	}).then((response) => {
			if (response.data.success !== undefined) {
		  		let light_data = response.data.success.light;
		  		let roller_blind_data = response.data.success.roller_blind;
				setCurrentLightState(light_data);
				setCurrentRollerBlindState(roller_blind_data);
			}
		});
  	}

  	const changeStatus = () => {
		axios.post('/manage_room_state', { light: manageLight, rollerBlind : parseInt(manageRollerBlind) }, {
			headers: {
		  		'Content-Type': 'application/json',
		  		'Accept': 'application/json',
			},
	  	}).then((response) => {
			if (response.data.success) {
				setAlertState(true);
				setVisibleAlert(true);
			} else {
				setAlertState(false);
				setVisibleAlert(true);
			}
		});
  	}

  	const updateLightChart = () => {
		axios.post('/update_light_chart', {}, {
			headers: {
		  		'Content-Type': 'multipart/form-data',
		  		'Accept': 'application/json',
			},
	  	}).then((response) => {
			if (response.data.success !== undefined) {
		  		let light_data = response.data.success.light_data;
		  		if(light_data.length > 0){
					let new_label = light_data.map(ld => new Date(ld.date));
					let new_data = light_data.map(ld => ld.light ? 'ON' : 'OFF');
					let new_light_data = {
						label : new_label,
						data : new_data,
					}
					setLightData(new_light_data)
		  		}
			}
		});
  	}

  	const updateRollerBlindChart = () => {
		axios.post('/update_roller_blind_chart', {}, {
			headers: {
		  		'Content-Type': 'multipart/form-data',
		  		'Accept': 'application/json',
			},
	  	}).then((response) => {
			console.log(response.data)
			if (response.data.success !== undefined) {
		  		let roller_blind_data = response.data.success.roller_blind_data;
		  		if(roller_blind_data.length > 0){
					let new_label = roller_blind_data.map(rbd => new Date(rbd.date));
					let new_data = roller_blind_data.map(rbd => rbd.roller_blind);
					let new_roller_blind_data = {
						label : new_label,
						data : new_data,
					}
					setRollerBlindData(new_roller_blind_data);
		  		}
			}
		})
  	}

  	return ( <>
		<CContainer fluid style={{backgroundColor : '#D3D3D3'}}>
			<CRow>
				<h1 className='heading'>Smart Room Dashboard</h1>
			</CRow>
			<CRow>
				<CAlert color={alertState ? 'success' : 'danger'} dismissible visible={visibleAlert} onClose={() => setVisibleAlert(false)}>
					{alertState ? 'Room state changed successfully' : 'An error occurred while trying to change room state, retry'}
				</CAlert>
				<CCol>
					<CCard className='current-status'>
						<CCardHeader>
							<h2>Current Room State</h2>
						</CCardHeader>
						<CCardBody>
							<p><b>Roller Blind: </b> <CBadge color={currentRollerBlindState === null ? 'warning' : 'info'}>{currentRollerBlindState === null ? 'NULL' : currentRollerBlindState}</CBadge></p>
							<p><b>Light: </b> <CBadge color={currentLightState === null ? 'warning' : currentLightState ? 'success' : 'danger'}>{currentLightState === null ? 'NULL' : currentLightState ? 'ON' : 'OFF'}</CBadge></p>
						</CCardBody>
					</CCard>
				</CCol>
				<CCol>
					<CCard className='manage-status'>
						<CCardHeader><h2>Manage Room Status</h2></CCardHeader>
						<CCardBody>
							<CFormRange
        						label={'Roller Blind Level\t' + manageRollerBlind +'%'}
        						min={0}
        						max={100}
        						step={1}
								value={manageRollerBlind}
								onChange={(event) => setManageRollerBlind(event.target.value)}
    						></CFormRange>
							<CFormSwitch
								size='lg'
								label='Light'
								defaultValue={false}
								onClick={(event) => setManageLight(event.target.checked)}
      						></CFormSwitch>
						</CCardBody>
						<CCardFooter>
							<CButton
								color='primary'
								shape='rounded'
								type='button'
								onClick={() => changeStatus()}
							>Save
							</CButton>
						</CCardFooter>
					</CCard>
				</CCol>
			</CRow>
			<CRow>
				<CCol>
					<CCard className='chart-light'>
						<CCardHeader>
							<CRow>
								<CCol sm={11}>
									<h2>Light State Chart</h2>
								</CCol>
								<CCol sm={1}>
									<CButton onClick={() => updateLightChart()}><CIcon icon={cilSync} size="lg" color="danger"/></CButton>
								</CCol>
							</CRow>
						</CCardHeader>
						<CCardBody>
							<Line 
								data={{
									labels : lightData.label,
									datasets : [
										{
											label: 'Light Data',
											data: lightData.data,
											backgroundColor: '#E46651',
											borderColor: '#E46651',
											pointHoverBackgroundColor: '#E46651',
											stepped: true,
										}
									]
								}}
								options={{
									responsive : true,
									plugins: {
										title : {
											display : true,
											text : 'Light State'
										}			
									},
									scales : {
										x : {
											type : 'time',
											time: {
												parser: 'MM/dd/yyyy HH:mm:ss',
              									tooltipFormat: 'MM/dd/yyyy HH:mm:ss',
              									unit: 'second',
              									unitStepSize: 1,
              									displayFormats: {
                									'second': 'MM/dd/yyyy HH:mm:ss'
              									}
											},
											adapters : {
												date : {
													locale : it,
												}
											}
										},
										y : {
											type : 'category',
											labels : ['ON', 'OFF'],
											offset : true,
										}
									},
									animation : false,
									elements: {
										line: {
										  borderWidth: 2,
										  tension: 0,
										},
										point: {
										  radius: 4,
										  hitRadius: 10,
										  hoverRadius: 4,
										},
									},
								}}
							/>
						</CCardBody>
					</CCard>
				</CCol>
			</CRow>
			<CRow>
				<CCol>
					<CCard className='chart-roller'>
						<CCardHeader>
							<CRow>
								<CCol sm={11}>
									<h2>Roller Blind State Chart</h2>
								</CCol>
								<CCol sm={1}>
									<CButton onClick={() => updateRollerBlindChart()}><CIcon icon={cilSync} size="lg"/></CButton>
								</CCol>
							</CRow>
						</CCardHeader>
						<CCardBody>
						<Line 
								data={{
									labels : rollerBlindData.label,
									datasets : [
										{
											label: 'Roller Blind Data',
											data: rollerBlindData.data,
											backgroundColor: '#E46651',
											borderColor: '#E46651',
											pointHoverBackgroundColor: '#E46651',
										}
									]
								}}
								options={{
									responsive : true,
									plugins: {
										title : {
											display : true,
											text : 'Roller Blind State'
										}			
									},
									scales : {
										x : {
											type : 'time',
											time: {
												parser: 'MM/dd/yyyy HH:mm:ss',
              									tooltipFormat: 'MM/dd/yyyy HH:mm:ss',
              									unit: 'second',
              									unitStepSize: 1,
              									displayFormats: {
                									'second': 'MM/dd/yyyy HH:mm:ss'
              									}
											},
											adapters : {
												date : {
													locale : it,
												}
											}
										},
										y : {
											offset : true,
										}
									},
									animation : false,
									elements: {
										line: {
										  borderWidth: 2,
										  tension: 0,
										},
										point: {
										  radius: 4,
										  hitRadius: 10,
										  hoverRadius: 4,
										},
									},
								}}
							/>
						</CCardBody>
					</CCard>
				</CCol>
			</CRow>
		</CContainer>
		</>
  	);
}

export default App;
